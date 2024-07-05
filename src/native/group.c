#include <quickjs.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include "color.h"
#include "group.h"
#include "native.h"



struct native_group_s {
  char *group;
};


static void update_native_group(JSContext *ctx, JSValue this_val, JSValue props)
{
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_group_s *nt = s->priv;

  
}



static void render_native_group(JSContext *ctx, JSValue this_val)
{

  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);
  struct native_group_s *nt= s->priv;

  igBeginGroup();
  render_children(ctx, this_val);
  igEndGroup();
  
}



static void create_native_group(JSContext *ctx, JSValue this_val, JSValue props)
{
  struct native_func_s *s = JS_GetOpaque(this_val, js_native_class_id);

  
  struct native_group_s *nt;

  nt = (struct native_group_s *)malloc(sizeof(struct native_group_s));
  memset(nt, 0, sizeof(struct native_group_s));
  s->priv = nt;
  update_native_group(ctx, this_val, props);

}

static void delete_native_group(JSContext *ctx, JSValue this_val)
{
  printf("delete group\n");

}

static void free_native_group(void *arg)
{
  struct native_group_s *nt = arg;
  
  free(arg);
}

struct native_func_s native_group = {
  .type = "group",
  .create_native = create_native_group,
  .delete_native = delete_native_group,
  .update_native = update_native_group,
  .render_native = render_native_group,
  .free_native = free_native_group
};
