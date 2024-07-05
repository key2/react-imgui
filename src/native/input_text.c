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
  JSValue onChangeText;
  JSValue value;
  JSValue PlaceHolder;
  JSValue size;
  char *text;
};


static void update_native_input_text(JSContext *ctx, JSValue this_val, JSValue props)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_text_s *nt = s->priv;
  JSValue children, tmp;

  
  nt->onChangeText = JS_GetPropertyStr(ctx, props, "onChangeText");

  /*
    nt->PlaceHolder = JS_GetPropertyStr(ctx, props, "PlaceHolder");
  */
  native_get_color(ctx, props,   nt->colors);

}

static void render_native_input_text(JSContext *ctx, JSValue this_val)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_text_s *nt=s->priv;


  igPushID_Ptr(nt);
  native_push_color(nt->colors);




  //TODO FIX ME!!! size should not be 1000 but param
  if(igInputText("##edit", nt->text, 1000, ImGuiInputTextFlags_None, NULL, NULL)){

    if(JS_IsFunction(ctx, nt->onChangeText)){
      JSValue text[1];
      //nt->value = JS_GetPropertyStr(ctx, props, "value");
      
      text[0] = JS_NewString(ctx, nt->text);
      JS_Call(ctx, nt->onChangeText, this_val, 1, text);
      JS_FreeValue(ctx, text[0]);
    } 

  }
  
  native_pop_color(nt->colors);
  igPopID();


}



static void create_native_input_text(JSContext *ctx, JSValue this_val, JSValue props)
{
  
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_text_s *nt;

  nt = (struct native_text_s *)malloc(sizeof(struct native_text_s));
  memset(nt, 0, sizeof(struct native_text_s));
  s->priv = nt;
  nt->text = malloc(1000);
  memset(nt->text, 0, 1000);
  nt->colors = create_style(color_table, "text");
  nt->onChangeText = JS_UNDEFINED;
  
  update_native_input_text(ctx, this_val, props);


}

static void delete_native_input_text(JSContext *ctx, JSValue this_val)
{

  printf("delete text\n");


}

static void free_native_input_text(void *arg)
{
  struct native_text_s *nt = arg;
  if(nt->text)
    free(nt->text);
  if(nt->colors)
    free(nt->colors);
  
  free(arg);
}

struct native_func_s native_input_text = {
  .type = "input",
  .create_native = create_native_input_text,
  .delete_native = delete_native_input_text,
  .update_native = update_native_input_text,
  .render_native = render_native_input_text,
  .free_native = free_native_input_text
};
