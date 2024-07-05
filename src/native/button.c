#include <quickjs.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include "color.h"
#include "button.h"
#include "native.h"
#include "style.h"


struct native_button_s {
  struct style_val_s **colors;

  
  ImVec2 bsize;
  char title[100];
  JSValue onPress;
};


static void update_native_button(JSContext *ctx, JSValue this_val, JSValue props)
{


  JSValue  title, size;


  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_button_s *nb = s->priv;
  
  
  nb->bsize.x=100;
  nb->bsize.y = 50;
 

  title = JS_GetPropertyStr(ctx, props, "title");
  const char *buf = NULL;
  size_t plen;
  if(JS_IsString(title)){
    strncpy(nb->title, JS_ToCStringLen(ctx, &plen, title), 100);
  }
  JS_FreeValue(ctx, title);
 
  if (nb->title[0] ==0 )
    strcpy(nb->title, "Button");
   
  size = JS_GetPropertyStr(ctx, props, "size");
  if(JS_IsObject(size)){
    JSValue sm;
    int x, y;
    sm = JS_GetPropertyStr(ctx, size, "width");
    if(JS_IsNumber(sm)){
      JS_ToUint32(ctx, &x, sm);
      nb->bsize.x = x;
    }
    JS_FreeValue(ctx, sm);

    sm = JS_GetPropertyStr(ctx, size, "height");
    JS_FreeValue(ctx, size);
    
    if(JS_IsNumber(sm)){
      JS_ToUint32(ctx, &y, sm);
      nb->bsize.y = y;
    }
    JS_FreeValue(ctx, sm);  
  }


  nb->onPress = JS_GetPropertyStr(ctx, props, "onPress");
  
  native_get_color(ctx, props,   nb->colors);


}



static void render_native_button(JSContext *ctx, JSValue this_val)
{


  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_button_s *button = s->priv;

  igPushID_Ptr(button);

  native_push_color(button->colors);
  
  if(igButton(button->title, button->bsize)){
    if(JS_IsFunction(ctx, button->onPress)){
      JS_Call(ctx, button->onPress, this_val, 0, NULL);
    } 
  }

  native_pop_color(button->colors);
    
  igPopID();
}



static void create_native_button(JSContext *ctx, JSValue this_val, JSValue props)
{
  
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_button_s *nb = s->priv;
  
  nb = (struct native_button_s *)malloc(sizeof(struct native_button_s));
  memset(nb, 0, sizeof(struct native_button_s));
  s->priv = nb;
  nb->colors = create_style(color_table, "button");
  nb->onPress = JS_UNDEFINED;
  
  update_native_button(ctx, this_val, props);


  
}

static void delete_native_button(JSContext *ctx, JSValue this_val)
{
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_button_s *nb = s->priv;

  printf("delete button\n");
  
}


static void free_native_button(void *arg)
{
  struct native_button_s *nb = arg;

  if(nb->colors)
    free_style(nb->colors);

  free(arg);
}


struct native_func_s native_button = {
  .type = "button",
  .create_native = create_native_button,
  .delete_native = delete_native_button,
  .update_native = update_native_button,
  .render_native = render_native_button,
  .free_native = free_native_button
};

