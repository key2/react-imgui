#include <quickjs.h>
#include <math.h>
#include <string.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include "color.h"
#include "native.h"
#include "style.h"

#include "button.h"
#include "text.h"
#include "unknown.h"



JSClassID js_native_class_id;

#define countof(x) (sizeof(x) / sizeof((x)[0]))


const char *native_get_style(JSContext *ctx, JSValue props, const char *type)
{
  JSValue style, attr;

  if(!JS_IsObject(props)){
    return NULL;
  }
  style = JS_GetPropertyStr(ctx, props, "style");
  if(!JS_IsObject(style)){
    return NULL;
  }
  attr = JS_GetPropertyStr(ctx, style, type);
  JS_FreeValue(ctx, style);
  size_t plen;
  if(JS_IsString(attr)){
    JS_FreeValue(ctx, attr);
    return JS_ToCStringLen(ctx, &plen, attr);
  }
  JS_FreeValue(ctx, attr);
  return NULL;
}

void native_get_color(JSContext *ctx, JSValue props, struct style_val_s **st_lst)
{
  const char *color;
  struct style_val_s **p;

  printf("%p\n", st_lst);
  for(p=st_lst; *p; p++){

    color = native_get_style(ctx, props, (*p)->type);
    if(color){
      (*p)->present=1;
      (*p)->val = find_color(color);
    } else {
      (*p)->present=0;
    }
  }
}

void native_push_color(struct style_val_s **st_lst)
{
  struct style_val_s **p;
  for(p=st_lst; *p; p++){
    if((*p)->present){
      
      igPushStyleColor_U32((*p)->imgui_val, (*p)->val);
    }
  }
}


void native_pop_color(struct style_val_s **st_lst)
{
  struct style_val_s **p;
  int cnt=0;
  
  for(p=st_lst; *p; p++){
    if((*p)->present){
      cnt++;
    }
  }
  igPopStyleColor(cnt);
}


void render_children(JSContext *ctx, JSValueConst this_val)                           
{
  JSValue children, tmp, fun;
  
  children = JS_GetPropertyStr(ctx, this_val, "children");  
  if (JS_IsException(children)){
    printf("pas de children\n");
  }
  uint32_t arrlen;
  if(JS_IsArray(ctx, children)){
    tmp = JS_GetPropertyStr(ctx, children, "length");
    JS_ToUint32(ctx, &arrlen, tmp);
    //printf("array len = %d\n", arrlen);
    JS_FreeValue(ctx, tmp);
    for(int i =0; i < arrlen; i++){
      tmp = JS_GetPropertyUint32(ctx, children, i);
      fun = JS_GetPropertyStr(ctx, tmp, "render");
      if(JS_IsFunction(ctx, fun)){
	JS_Call(ctx, fun, tmp, 0, NULL);
      }
      JS_FreeValue(ctx, tmp);
      JS_FreeValue(ctx, fun);
    }
  }
  JS_FreeValue(ctx, children);  
}


struct native_func_s *find_native_fun(const char *name)
{
  struct native_func_s *nf;

  for(int i = 0; i < countof(native_table); i++){
    nf = native_table[i];
    if(!strcmp(name, nf->type))
      return nf;
  }

  printf("DIDNT FIND %s\n\n", name);
  /* Did not find, send unknown !*/  
  for(int i = 0; i < countof(native_table); i++){
    nf = native_table[i];
    if(!strcmp("unknown", nf->type))
      return nf;
  }
  return NULL;
}


static void js_native_finalizer(JSRuntime *rt, JSValue val)
{
    struct native_func_s *s = JS_GetOpaque(val, js_native_class_id);
    printf("\n\nHEEEEEEEEEEEEEEU WE DESTRAYED %p\n\n", s);

    s->free_native(s->priv);
    /* Note: 's' can be NULL in case JS_SetOpaque() was not called */
    js_free_rt(rt, s);
}



static JSValue js_native_ctor(JSContext *ctx,
                             JSValueConst new_target,
                             int argc, JSValueConst *argv)
{
  struct native_func_s *s;
    JSValue obj = JS_UNDEFINED;
    JSValue proto;
    JSValue type;
    
    if(!JS_IsString(argv[0])){
      return JS_EXCEPTION;
    }
    if(!JS_IsObject(argv[1])){
      return JS_EXCEPTION;
    }


    const char *buf;
    size_t plen;
    buf = JS_ToCStringLen(ctx, &plen, argv[0]);
    
    printf("Nouvelle classe constructor %s!!!\n", buf);

    
    s = js_mallocz(ctx, sizeof(struct native_func_s));
    if (!s)
        return JS_EXCEPTION;

    /* we copy the type */
    memcpy(s, find_native_fun(buf), sizeof(struct native_func_s));

    /* then we initialize it */
    
    proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto))
        goto fail;    

    obj = JS_NewObjectProtoClass(ctx, proto, js_native_class_id);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        goto fail;

    //JS_SetPropertyStr(ctx, obj, "type", JS_DupValue(ctx, argv[0]));
    JS_SetPropertyStr(ctx, obj, "props", JS_DupValue(ctx, argv[1]));
    JS_SetPropertyStr(ctx, obj, "children",  JS_NewArray(ctx));

    JS_SetOpaque(obj, s);

    
    /* Initialize object here !*/
    s->create_native(ctx, obj, argv[1]);  
    

    return obj;
 fail:
    js_free(ctx, s);
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}






static JSValue js_native_render(JSContext *ctx, JSValueConst this_val,
                             int argc, JSValueConst *argv)
{
  
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);

  s->render_native(ctx, this_val);

  return JS_NULL;
}



static JSValue js_native_update(JSContext *ctx, JSValueConst this_val,
                             int argc, JSValueConst *argv)
{
  
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);

  s->update_native(ctx, this_val, argv[0]);
  
  //printf("Updating !!!\n");
  
  return JS_NULL;
}




static JSValue js_native_delete(JSContext *ctx, JSValueConst this_val,
                             int argc, JSValueConst *argv)
{
  
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);

  s->delete_native(ctx, this_val);
  
  //printf("Updating !!!\n");
  
  return JS_NULL;
}




static JSClassDef js_native_class = {
    "NativeElement",
    .finalizer = js_native_finalizer,
};

static const JSCFunctionListEntry js_native_proto_funcs[] = {
  JS_CFUNC_DEF("render", 0, js_native_render),
  JS_CFUNC_DEF("update", 1, js_native_update),
  JS_CFUNC_DEF("delete", 1, js_native_delete),
};

static int js_native_init(JSContext *ctx, JSModuleDef *m)
{
    JSValue native_proto, native_class;

    JS_NewClassID( &js_native_class_id);
    JS_NewClass(JS_GetRuntime(ctx), js_native_class_id, &js_native_class);

    native_proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, native_proto, js_native_proto_funcs, countof(js_native_proto_funcs));

    native_class = JS_NewCFunction2(ctx, js_native_ctor, "NativeElement", 2, JS_CFUNC_constructor, 0);
    /* set proto.constructor and ctor.prototype */
    JS_SetConstructor(ctx, native_class, native_proto);
    JS_SetClassProto(ctx, js_native_class_id, native_proto);

    JS_SetModuleExport(ctx, m, "NativeElement", native_class);
    return 0;
}

JSModuleDef *js_init_module(JSContext *ctx, const char *module_name)
{
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, js_native_init);
    if (!m)
        return NULL;
    JS_AddModuleExport(ctx, m, "NativeElement");
    return m;
}

/*
int main()
{
  JSValue props;
  void (*render)(JSContext *ctx, JSValueConst this_val);
  render = find_renderer("view");
  render(NULL, props);
}
*/
