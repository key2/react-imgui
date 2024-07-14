#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <quickjs.h>
#include "color.h"
#include "text.h"
#include "native.h"
#include "style.h"


struct native_text_s {

  struct style_val_s **colors;
  JSValue onChange;
  bool val;
  char label[100];
};


static void update_native_checkbox(JSContext *ctx, JSValue this_val, JSValue props)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_text_s *nt = s->priv;
  JSValue children, tmp, label, value;

  value = JS_GetPropertyStr(ctx, props, "value");
  if(JS_IsBool(value)){
    nt->val = JS_ToBool(ctx,value);
  }
  
  nt->onChange = JS_GetPropertyStr(ctx, props, "onChange");
  label = JS_GetPropertyStr(ctx, props, "label");
  nt->label[0] = '\0';
  const char *buf = NULL;
  size_t plen;
  if(JS_IsString(label)){
    buf = JS_ToCStringLen(ctx, &plen, label);
    strncpy(nt->label, buf, 100);
  }
  
  
  /*
    nt->PlaceHolder = JS_GetPropertyStr(ctx, props, "PlaceHolder");
  */
  
  //native_get_color(ctx, props,   nt->colors);

}

static void render_native_checkbox(JSContext *ctx, JSValue this_val)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_text_s *nt=s->priv;

  
  igPushID_Ptr(nt);
  native_push_color(nt->colors);
  
  if(igCheckbox(nt->label, &nt->val)){

    if(JS_IsFunction(ctx, nt->onChange)){
      JSValue val[1];
      val[0] = JS_NewBool(ctx, nt->val);
      JS_Call(ctx, nt->onChange, this_val, 1, val);
      JS_FreeValue(ctx, val[0]);
    } 
  }
  
  native_pop_color(nt->colors);
  igPopID();


}



static void create_native_checkbox(JSContext *ctx, JSValue this_val, JSValue props)
{
  
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_text_s *nt;

  nt = (struct native_text_s *)malloc(sizeof(struct native_text_s));
  memset(nt, 0, sizeof(struct native_text_s));
  s->priv = nt;
  
  nt->onChange = JS_UNDEFINED; 
  nt->colors = create_style(color_table, "check");
  update_native_checkbox(ctx, this_val, props);

}

static void delete_native_checkbox(JSContext *ctx, JSValue this_val)
{

  printf("delete text\n");


}

static void free_native_checkbox(void *arg)
{
  struct native_text_s *nt = arg;

  if(nt->colors)
    free(nt->colors);
  
  free(arg);
}

struct native_func_s native_checkbox = {
  .type = "checkbox",
  .create_native = create_native_checkbox,
  .delete_native = delete_native_checkbox,
  .update_native = update_native_checkbox,
  .render_native = render_native_checkbox,
  .free_native = free_native_checkbox
};
