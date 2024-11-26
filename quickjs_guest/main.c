// Included from hyperlight_guest_capi/include
#include "hyperlight_guest.h"
// Included from hyperlight_guest/third_party/libc
#include <stdint.h>
#include <stdio.h>
#include <string.h>
// Included from hyperlight_guest/third_party/printf
#include "printf.h"
#include "quickjs.h"

int guest_function(const char *code) {
    JSRuntime *rt;
    JSContext *ctx;
    JSValue result;
    int ret;
    size_t len;

    rt = JS_NewRuntime();
    JS_SetMemoryLimit(rt, 16 * 1024 * 1024); // 16MB, half of what we allow the total sandbox to use
    JS_SetMaxStackSize(rt, 16 * 1024 * 1024); // 16MB, half of what we allow the total sandbox to use
    ctx = JS_NewContext(rt);

    // Evaluate a JavaScript expression
    result = JS_Eval(ctx, code, strlen(code), "<eval>", JS_EVAL_TYPE_GLOBAL);
    const char *st = JS_ToCString(ctx, result);

    // Check for errors
    if (JS_IsException(result)) {
        JSValue exception = JS_GetException(ctx);
        printf("Error: %s\n", JS_ToCString(ctx, exception));
        JS_FreeValue(ctx, exception);
        ret = 1;
    } else {
        // Print the result
        int32_t value;
        JS_ToInt32(ctx, &value, result);
        printf("%s\n", st);
        ret = 0;
    }

    JS_FreeValue(ctx, result);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);

    return ret;
}

HYPERLIGHT_WRAP_FUNCTION(guest_function, Int, 1, String);

void hyperlight_main(void) {
    HYPERLIGHT_REGISTER_FUNCTION("EvalScript", guest_function);
}

// This dispatch function is only used when the host dispatches a guest function
// call but there is no registered guest function with the given name.
hl_Vec *c_guest_dispatch_function(const hl_FunctionCall *function_call) {
    return NULL;
}
