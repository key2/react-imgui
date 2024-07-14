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

  char *text;
};


static void update_native_text(JSContext *ctx, JSValue this_val, JSValue props)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_text_s *nt = s->priv;
  JSValue children, tmp;
  
  children = JS_GetPropertyStr(ctx, this_val, "children");
  
  if(JS_IsArray(ctx, children)){
    JSValue len;
    uint32_t arrlen;
    len = JS_GetPropertyStr(ctx, children, "length");
    JS_ToUint32(ctx, &arrlen, len);    

    printf("text array len %d\n", arrlen);
    JS_FreeValue(ctx, len);

    if(arrlen > 1){
      printf("CHECK MEEEEEEEEEEEEEEEEEEEEEEEEEEE\n");
    }
    if(arrlen > 0){
      tmp = JS_GetPropertyUint32(ctx, children, 0);
      const char *buf = NULL;
      size_t plen;
      if(JS_IsString(tmp)){
	buf = JS_ToCStringLen(ctx, &plen, tmp);
	if(nt->text){
	  free(nt->text);
	}
	nt->text = strndup(buf, plen);
      }
     
      JS_FreeValue(ctx, tmp);
    } else {
      if(nt->text){
	free(nt->text);
      }
      nt->text = strdup("text");
    }
  }

  // JS_FreeValue(ctx, children);

  native_get_color(ctx, props,   nt->colors);

}



static void render_native_text(JSContext *ctx, JSValue this_val)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_text_s *nt=s->priv;

  native_push_color(nt->colors);

  igText("%s", nt->text);
  
  native_pop_color(nt->colors);
}



static void create_native_text(JSContext *ctx, JSValue this_val, JSValue props)
{
  
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_text_s *nt;

  nt = (struct native_text_s *)malloc(sizeof(struct native_text_s));
  memset(nt, 0, sizeof(struct native_text_s));
  s->priv = nt;
  nt->colors = create_style(color_table, "text");
  update_native_text(ctx, this_val, props);

  

}

static void delete_native_text(JSContext *ctx, JSValue this_val)
{

  printf("delete text\n");


}

static void free_native_text(void *arg)
{
  struct native_text_s *nt = arg;
  if(nt->text)
    free(nt->text);
  if(nt->colors)
    free(nt->colors);
  
  free(arg);
}

struct native_func_s native_text = {
  .type = "text",
  .create_native = create_native_text,
  .delete_native = delete_native_text,
  .update_native = update_native_text,
  .render_native = render_native_text,
  .free_native = free_native_text
};
