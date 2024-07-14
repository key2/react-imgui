#include <quickjs.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include "color.h"
#include "radio.h"
#include "native.h"

struct radio_chain {
  JSContext *ctx;
  int *val;
  struct radio_chain *next;
  struct radio_chain *prev;
  
};

static struct radio_chain chain = {NULL, 0, NULL, NULL};


static void add_radio_container_chain(JSContext *ctx, int *val)
{
  struct radio_chain *c;

  c= (struct radio_chain *)malloc(sizeof(struct radio_chain));
  memset(c, 0, sizeof(struct radio_chain));
  c->next = chain.next;
  if(chain.next)
    chain.next->prev = c;
  chain.next = c;
  c->prev = &chain;
  c->ctx = ctx;
  c->val = val;
}

static void del_radio_container_chain(JSContext *ctx)
{
  struct radio_chain *c;
  for(c=chain.next; c; c=c->next){
    if(c->ctx == ctx){
      c->prev->next = c->next;
      if(c->next)
	c->next->prev = c->prev;
      free(c);
      return;
    }
  }
}

struct radio_chain *find_radio_container_chain(JSContext *ctx)
{
  struct radio_chain *c;
  for(c=chain.next; c; c=c->next){
    if(c->ctx == ctx){
      return c;
    }
  }
  return NULL;
}

struct native_radio_container_s {
  char *radio_container;
  int val;
  JSValue onChange;
};


static void update_native_radio_container(JSContext *ctx, JSValue this_val, JSValue props)
{
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_radio_container_s *nt = s->priv;

  
  nt->onChange = JS_GetPropertyStr(ctx, props, "onChange");
  
}



static void render_native_radio_container(JSContext *ctx, JSValue this_val)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_radio_container_s *nt= s->priv;
  
  int oldval;

  oldval = nt->val;
  
  /* Is it thread safe ?? */
  add_radio_container_chain(ctx, &nt->val);
  render_children(ctx, this_val);
  del_radio_container_chain(ctx);

  if(oldval != nt->val){
    if(JS_IsFunction(ctx, nt->onChange)){
      JSValue val[1];
      
      val[0] = JS_NewInt32(ctx, (int)nt->val);
      JS_Call(ctx, nt->onChange, this_val, 1, val);
      JS_FreeValue(ctx, val[0]);
    } 

    
  }
  
}



static void create_native_radio_container(JSContext *ctx, JSValue this_val, JSValue props)
{
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);

  
  struct native_radio_container_s *nt;

  nt = (struct native_radio_container_s *)malloc(sizeof(struct native_radio_container_s));
  memset(nt, 0, sizeof(struct native_radio_container_s));
  s->priv = nt;
  nt->onChange = JS_UNINITIALIZED;

  update_native_radio_container(ctx, this_val, props);

}

static void delete_native_radio_container(JSContext *ctx, JSValue this_val)
{
  printf("delete radio_container\n");

}

static void free_native_radio_container(void *arg)
{
  struct native_radio_container_s *nt = arg;
  
  free(arg);
}

struct native_func_s native_radio_container = {
  .type = "radio_container",
  .create_native = create_native_radio_container,
  .delete_native = delete_native_radio_container,
  .update_native = update_native_radio_container,
  .render_native = render_native_radio_container,
  .free_native = free_native_radio_container
};



struct native_radio_button_s {
  char *radio_button;
  struct style_val_s **colors;
  char label[100];
  int val;
};


static void update_native_radio_button(JSContext *ctx, JSValue this_val, JSValue props)
{
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_radio_button_s *nt = s->priv;
  JSValue label, val;
  
  label = JS_GetPropertyStr(ctx, props, "label");
  const char *buf = NULL;
  size_t plen;
  if(JS_IsString(label)){
    strncpy(nt->label, JS_ToCStringLen(ctx, &plen, label), 99);
  }
  JS_FreeValue(ctx, label);

  val = JS_GetPropertyStr(ctx, props, "value");
  if(JS_IsNumber(val)){
    JS_ToInt32(ctx, &nt->val, val);
  }
}



static void render_native_radio_button(JSContext *ctx, JSValue this_val)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_radio_button_s *nt= s->priv;

  struct radio_chain *c;

  c= find_radio_container_chain(ctx);
  if(c){
    
    igPushID_Ptr(nt);

    //native_push_color(nt->colors);
    
    if(igRadioButton_IntPtr(nt->label, c->val , nt->val)){
      //printf("changed %d\n", nt->val);
    }
    
    //native_pop_color(nt->colors);
    
    igPopID();
  }
  
}



static void create_native_radio_button(JSContext *ctx, JSValue this_val, JSValue props)
{
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);

  
  struct native_radio_button_s *nt;

  nt = (struct native_radio_button_s *)malloc(sizeof(struct native_radio_button_s));
  memset(nt, 0, sizeof(struct native_radio_button_s));
  s->priv = nt;
  strcpy(nt->label, "Check Button");
  update_native_radio_button(ctx, this_val, props);

}

static void delete_native_radio_button(JSContext *ctx, JSValue this_val)
{
  printf("delete radio_button\n");

}

static void free_native_radio_button(void *arg)
{
  struct native_radio_button_s *nt = arg;
  
  free(arg);
}

struct native_func_s native_radio_button = {
  .type = "radio_button",
  .create_native = create_native_radio_button,
  .delete_native = delete_native_radio_button,
  .update_native = update_native_radio_button,
  .render_native = render_native_radio_button,
  .free_native = free_native_radio_button
};
