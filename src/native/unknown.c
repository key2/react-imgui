#include <quickjs.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include "color.h"
#include "unknown.h"
#include "native.h"



struct native_unknown_s {
  char *unknown;
};


static void update_native_unknown(JSContext *ctx, JSValue this_val, JSValue props)
{
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_unknown_s *nt = s->priv;

  
}



static void render_native_unknown(JSContext *ctx, JSValue this_val)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_unknown_s *nt= s->priv;
  render_children(ctx, this_val);

}



static void create_native_unknown(JSContext *ctx, JSValue this_val, JSValue props)
{
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);

  
  struct native_unknown_s *nt;

  nt = (struct native_unknown_s *)malloc(sizeof(struct native_unknown_s));
  memset(nt, 0, sizeof(struct native_unknown_s));
  s->priv = nt;
  update_native_unknown(ctx, this_val, props);
}

static void delete_native_unknown(JSContext *ctx, JSValue this_val)
{
  printf("delete unknown\n");

}

static void free_native_unknown(void *arg)
{
  struct native_unknown_s *nt = arg;
  
  free(arg);
}

struct native_func_s native_unknown = {
  .type = "unknown",
  .create_native = create_native_unknown,
  .delete_native = delete_native_unknown,
  .update_native = update_native_unknown,
  .render_native = render_native_unknown,
  .free_native = free_native_unknown
};
