#include <quickjs.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  JSRuntime *rt;
  JSContext *ctx;
  JSValue result;
  int ret;

  rt = JS_NewRuntime();
  ctx = JS_NewContext(rt);

  char* code = "function fib(n) { if (n <= 1) return n; return fib(n - 1) + fib(n - 2); } fib(11);";

  // Evaluate a JavaScript expression
  result = JS_Eval(ctx, code, strlen(code), "<eval>", JS_EVAL_TYPE_GLOBAL);

  // Check for errors
  if (JS_IsException(result)) {
    JSValue exception = JS_GetException(ctx);
    fprintf(stderr, "Error: %s\n", JS_ToCString(ctx, exception));
    JS_FreeValue(ctx, exception);
    ret = 1;
  } else {
    // Print the result
    int32_t value;
    JS_ToInt32(ctx, &value, result);
    printf("Result: %d\n", value);
    ret = 0;
  }

  JS_FreeValue(ctx, result);
  JS_FreeContext(ctx);
  JS_FreeRuntime(rt);

  return ret;
}