#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <quickjs.h>
#include "color.h"
#include "native.h"
#include "style.h"
#include "vslider.h"

struct native_slider_s {
  bool is_float;
  
  struct style_val_s **colors;
  JSValue onChange;
  JSValue value;
  ImVec2 bsize;

  double val_min;
  double val_max;
  double val;
  double prev_val;
};


static void update_native_vslider(JSContext *ctx, JSValue this_val, JSValue props)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_slider_s *nt = s->priv;
  JSValue size, bounds;

  
  nt->onChange = JS_GetPropertyStr(ctx, props, "onChange");
  native_get_color(ctx, props,   nt->colors);

  
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

  
  
  bounds = JS_GetPropertyStr(ctx, props, "bounds");
  if(JS_IsObject(bounds)){
    JSValue sm;
    {
      sm = JS_GetPropertyStr(ctx, bounds, "min");
      if(JS_IsNumber(sm)){
	int32_t tmp;
	JS_ToInt32(ctx, &tmp, sm);
	nt->val_min = tmp;
      }
      if(JS_IsBigFloat(sm)){
	JS_ToFloat64(ctx, &nt->val_min, sm);
      }
      JS_FreeValue(ctx, sm);
    }

    {
      sm = JS_GetPropertyStr(ctx, bounds, "max");
      JS_FreeValue(ctx, bounds);
      if(JS_IsNumber(sm)){
	int32_t tmp;
	JS_ToInt32(ctx, &tmp, sm);
	nt->val_max = tmp;
      }
      if(JS_IsBigFloat(sm)){
	JS_ToFloat64(ctx, &nt->val_max, sm);
      }
      JS_FreeValue(ctx, sm);
    }
  }
 
}



static void render_native_vslider(JSContext *ctx, JSValue this_val)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_slider_s *nt=s->priv;


  igPushID_Ptr(nt);
  native_push_color(nt->colors);

  nt->prev_val=nt->val;
  
  if(!nt->is_float){
    int tmp;
    tmp=nt->val;
    igVSliderInt("##int", nt->bsize, &tmp, (int)nt->val_min, (int)nt->val_max, "%d", 0);
    nt->val = tmp;
  } else {
    float tmp;
    tmp = nt->val;
    igVSliderFloat("##float",nt->bsize, &tmp, nt->val_min, nt->val_max, "%.3f", 0);
    nt->val=tmp;
  }
  
  native_pop_color(nt->colors);
  igPopID();
  
  //igSameLine(0.0f, -1.0f);

  if(nt->prev_val != nt->val){
    
    if(JS_IsFunction(ctx, nt->onChange)){
      JSValue val[1];
      //nt->value = JS_GetPropertyStr(ctx, props, "value");
      if(nt->is_float)
	val[0] = JS_NewFloat64(ctx, nt->val);
      else
	val[0] = JS_NewInt32(ctx, (int)nt->val);
      
      JS_Call(ctx, nt->onChange, this_val, 1, val);
      JS_FreeValue(ctx, val[0]);
    }
  }

  
}



static void create_native_vslider(JSContext *ctx, JSValue this_val, JSValue props)
{
  
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_slider_s *nt;

  nt = (struct native_slider_s *)malloc(sizeof(struct native_slider_s));
  memset(nt, 0, sizeof(struct native_slider_s));
  s->priv = nt;

  nt->bsize.x = 18;
  nt->bsize.y = 160;

  nt->val=50;
  nt->val_min=0;
  nt->val_max=100;
  nt->is_float=true;
  nt->colors = create_style(color_table, "slider");
  nt->colors = add_style(nt->colors, color_table, "frame");
  nt->onChange = JS_UNDEFINED;
  
  update_native_vslider(ctx, this_val, props);


}

static void delete_native_vslider(JSContext *ctx, JSValue this_val)
{

  printf("delete text\n");


}

static void free_native_vslider(void *arg)
{
  struct native_slider_s *nt = arg;

  if(nt->colors)
    free(nt->colors);
  
  free(arg);
}

struct native_func_s native_vslider = {
  .type = "vslider",
  .create_native = create_native_vslider,
  .delete_native = delete_native_vslider,
  .update_native = update_native_vslider,
  .render_native = render_native_vslider,
  .free_native = free_native_vslider
};
