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
  JSValue value;
  ImVec2 bsize;
  float *_values;
  size_t _values_cnt;
  uint32_t inc;
  uint32_t pos;
  char *text;
};


static void update_native_plot_line(JSContext *ctx, JSValue this_val, JSValue props)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_text_s *nt = s->priv;
  JSValue  vals, size, increment;

  vals = JS_GetPropertyStr(ctx, props, "values");
  if(JS_IsArray(ctx, vals)){
    JSValue len;
    uint32_t arrlen;
    printf("value changed\n");
    len = JS_GetPropertyStr(ctx, vals, "length");
    JS_ToUint32(ctx, &arrlen, len);  
    nt->_values_cnt=arrlen;
    if(nt->_values)
      free(nt->_values);

    nt->_values = (float*)malloc(nt->_values_cnt * sizeof(float));
    for(int i=0; i< nt->_values_cnt; i++){
      JSValue tmp;
      
      tmp = JS_GetPropertyUint32(ctx, vals, i);
      if(JS_IsNumber(tmp)){
        int32_t tmpf;
        JS_ToInt32(ctx, &tmpf, tmp);
        nt->_values[i] = tmpf;
      }
      if(JS_IsBigFloat(tmp)){
	double tmpd;
	JS_ToFloat64(ctx, &tmpd, tmp);
	nt->_values[i] = tmpd;
      }
      JS_FreeValue(ctx, tmp);
    }
  }

  increment = JS_GetPropertyStr(ctx, props, "increment");
  if(JS_IsNumber(increment)){
    JS_ToUint32(ctx, &nt->inc, increment);
    nt->inc = nt->inc % nt->_values_cnt;
  }

  JS_FreeValue(ctx, increment);
      
  size = JS_GetPropertyStr(ctx, props, "size");
  if(JS_IsObject(size)){
    JSValue sm;
    uint32_t x, y;
    sm = JS_GetPropertyStr(ctx, size, "width");
    if(JS_IsNumber(sm)){
      JS_ToUint32(ctx, &x, sm);
      nt->bsize.x = x;
    }
    JS_FreeValue(ctx, sm);

    sm = JS_GetPropertyStr(ctx, size, "height");
    JS_FreeValue(ctx, size);
    
    if(JS_IsNumber(sm)){
      JS_ToUint32(ctx, &y, sm);
      nt->bsize.y = y;
    }
    JS_FreeValue(ctx, sm);  
  }

  native_get_color(ctx, props,   nt->colors);

}

static void render_native_plot_line(JSContext *ctx, JSValue this_val)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_text_s *nt=s->priv;


  igPushID_Ptr(nt);
  native_push_color(nt->colors);
  nt->pos = (nt->pos + nt->inc) % nt->_values_cnt;
  igPlotLines_FloatPtr("my label", nt->_values, nt->_values_cnt, nt->pos, NULL, igGET_FLT_MAX(), igGET_FLT_MAX(), nt->bsize , sizeof(float));  
  native_pop_color(nt->colors);
  igPopID();
}



static void create_native_plot_line(JSContext *ctx, JSValue this_val, JSValue props)
{
  
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_text_s *nt;

  nt = (struct native_text_s *)malloc(sizeof(struct native_text_s));
  memset(nt, 0, sizeof(struct native_text_s));
  s->priv = nt;

  nt->colors = create_style(color_table, "plotline");
  
  update_native_plot_line(ctx, this_val, props);


}

static void delete_native_plot_line(JSContext *ctx, JSValue this_val)
{

  printf("delete text\n");


}

static void free_native_plot_line(void *arg)
{
  struct native_text_s *nt = arg;
  if(nt->text)
    free(nt->text);
  if(nt->colors)
    free(nt->colors);
  
  free(arg);
}

struct native_func_s native_plot_line = {
  .type = "plotline",
  .create_native = create_native_plot_line,
  .delete_native = delete_native_plot_line,
  .update_native = update_native_plot_line,
  .render_native = render_native_plot_line,
  .free_native = free_native_plot_line
};
