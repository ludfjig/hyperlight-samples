// Included from hyperlight_guest_capi/include
#include "hyperlight_guest.h"
// Included from hyperlight_guest/third_party/libc
#include <stdint.h>
#include <stdio.h>
#include <string.h>
// Included from hyperlight_guest/third_party/printf
#include "printf.h"
#include "quickjs.h"

int print_output(const char *message) {
  int res = printf("%s", message);

  return res;
}

int guest_function(const char *from_host) {
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

HYPERLIGHT_WRAP_FUNCTION(print_output, Int, 1, String);
HYPERLIGHT_WRAP_FUNCTION(guest_function, Int, 1, String);

void hyperlight_main(void) {
  HYPERLIGHT_REGISTER_FUNCTION("PrintOutput", print_output);
  HYPERLIGHT_REGISTER_FUNCTION("GuestMethod1", guest_function);
}

// This dispatch function is only used when the host dispatches a guest function
// call but there is no registered guest function with the given name.
hl_Vec *c_guest_dispatch_function(const hl_FunctionCall *function_call) {
  const char *func_name = function_call->function_name;
  if (strcmp(func_name, "ThisIsNotARealFunctionButTheNameIsImportant") == 0) {
    // This is special case for test `iostack_is_working
    return hl_flatbuffer_result_from_Int(99);
  }

  return NULL;
}
