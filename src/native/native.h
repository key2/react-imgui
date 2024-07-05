#ifndef __NATIVE_H_
#define __NATIVE_H_

#include "style.h"

extern  JSClassID js_native_class_id;

struct native_func_s
{
  char *type;
  void (*create_native)(JSContext *ctx, JSValue this_val, JSValue props);
  void (*delete_native)(JSContext *ctx, JSValue this_val);
  void (*update_native)(JSContext *ctx, JSValue this_val, JSValue props);
  void (*render_native)(JSContext *ctx, JSValue this_val);
  void (*free_native)(void *priv);
  void *priv;
};

const char *native_get_style(JSContext *ctx, JSValue props, const char *type);
void native_get_color(JSContext *ctx, JSValue props,  struct style_val_s **st_lst);
void render_children(JSContext *ctx, JSValueConst this_val);
void native_push_color(struct style_val_s **st_lst);
void native_pop_color(struct style_val_s **st_lst);


#include "button.h"
#include "unknown.h"
#include "text.h"
#include "input_text.h"
#include "vslider.h"
#include "sameline.h"
#include "group.h"

static struct native_func_s *native_table[] = {
  &native_button,
  &native_text,
  &native_unknown,
  &native_input_text,
  &native_vslider,
  &native_sameline,
  &native_group
};

#endif
