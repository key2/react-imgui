#include <quickjs.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include "color.h"
#include "sameline.h"
#include "native.h"



struct native_sameline_s {
  char *sameline;
};


static void update_native_sameline(JSContext *ctx, JSValue this_val, JSValue props)
{
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_sameline_s *nt = s->priv;
  
}




static void render_sameline_children(JSContext *ctx, JSValueConst this_val)                           
{
  JSValue children, tmp, fun;
  
  children = JS_GetPropertyStr(ctx, this_val, "children");  
  if (JS_IsException(children)){
    printf("pas de children\n");
  }
  int arrlen;
  if(JS_IsArray(ctx, children)){
    tmp = JS_GetPropertyStr(ctx, children, "length");
    JS_ToUint32(ctx, &arrlen, tmp);
    //printf("array len = %d\n", arrlen);
    JS_FreeValue(ctx, tmp);
    for(int i =0; i < arrlen; i++){
      tmp = JS_GetPropertyUint32(ctx, children, i);
      fun = JS_GetPropertyStr(ctx, tmp, "render");
      if(JS_IsFunction(ctx, fun)){
	if(i > 0)
	  igSameLine(0.0f, -1.0f);
	JS_Call(ctx, fun, tmp, 0, NULL);
      }
      JS_FreeValue(ctx, tmp);
      JS_FreeValue(ctx, fun);
    }
  }
  JS_FreeValue(ctx, children);  
}


static void render_native_sameline(JSContext *ctx, JSValue this_val)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_sameline_s *nt= s->priv;


  
  render_sameline_children(ctx, this_val);

}



static void create_native_sameline(JSContext *ctx, JSValue this_val, JSValue props)
{
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);

  
  struct native_sameline_s *nt;

  nt = (struct native_sameline_s *)malloc(sizeof(struct native_sameline_s));
  memset(nt, 0, sizeof(struct native_sameline_s));
  s->priv = nt;
  update_native_sameline(ctx, this_val, props);

}

static void delete_native_sameline(JSContext *ctx, JSValue this_val)
{
  printf("delete sameline\n");

}

static void free_native_sameline(void *arg)
{
  struct native_sameline_s *nt = arg;
  
  free(arg);
}

struct native_func_s native_sameline = {
  .type = "sameline",
  .create_native = create_native_sameline,
  .delete_native = delete_native_sameline,
  .update_native = update_native_sameline,
  .render_native = render_native_sameline,
  .free_native = free_native_sameline
};
