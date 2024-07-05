#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_USE_OPENGL3
#define CIMGUI_USE_GLFW
#include <string.h>
#include <cimgui.h>
#include "cimgui_impl.h"
#include "bytecode.h"
#include <quickjs.h>
#include <quickjs-libc.h>

JSContext *ctx; 

static JSContext *JS_NewCustomContext(JSRuntime *rt)
{
  ctx = JS_NewContextRaw(rt);
  if (!ctx)
    return NULL;
  JS_AddIntrinsicBaseObjects(ctx);
  JS_AddIntrinsicDate(ctx);
  JS_AddIntrinsicEval(ctx);
  JS_AddIntrinsicStringNormalize(ctx);
  JS_AddIntrinsicRegExp(ctx);
  JS_AddIntrinsicJSON(ctx);
  JS_AddIntrinsicProxy(ctx);
  JS_AddIntrinsicMapSet(ctx);
  JS_AddIntrinsicTypedArrays(ctx);
  JS_AddIntrinsicPromise(ctx);
  JS_AddIntrinsicBigInt(ctx);
  {
    extern JSModuleDef *js_init_module_os(JSContext *ctx, const char *name);
    js_init_module_os(ctx, "os");
  }
  return ctx;
}

//TODO make a struct with 1 ctx per item;
JSValue func;
JSValue global_obj;
JSValue myval;

void js_init()
{
  printf("JS Initialized\n");
  JSRuntime *rt;
  //  JSContext *ctx;
  
  rt = JS_NewRuntime();
  js_std_set_worker_new_context_func(JS_NewCustomContext);
  js_std_init_handlers(rt);
  JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);
  ctx = JS_NewCustomContext(rt);
  js_std_add_helpers(ctx, 0, NULL);
  js_std_eval_binary(ctx, qjsc_myapp, qjsc_myapp_size, 0);
  js_std_loop(ctx);

  global_obj = JS_GetGlobalObject(ctx);
  myval = JS_GetPropertyStr(ctx, global_obj, "myval");
  func =  JS_GetPropertyStr(ctx, myval, "render");
  if(JS_IsFunction(ctx, func)){
    printf("Oui Func");
  }
}

void js_render()
{
  
  char *str = "this.myval.render();";

  igText("Application average %.3f ms/frame (%.1f FPS)",
             1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
  /*
  JSValue global_obj = JS_GetGlobalObject(ctx);
  */
  
  //  JSValue result = JS_Eval(ctx, str, strlen(str), "<myfile>", JS_EVAL_TYPE_GLOBAL);

  JSValue result = JS_Call(ctx, func, myval, 0, NULL);
  //JSValue result;
  if (JS_IsException(result)) {
    JSValue realException = JS_GetException(ctx);
    printf("%s\n", JS_ToCString(ctx, realException));
  } 

  js_std_loop(ctx);
}

int js_finish(int argc, char **argv)
{
  //TODO: get out of the loop when finished and get out from here
  /*
  js_std_free_handlers(rt);
  JS_FreeContext(ctx);
  JS_FreeRuntime(rt);
  return 0;
  */
  return 0;
}

void js_close()
{
  printf("JS Closed\n");
}
