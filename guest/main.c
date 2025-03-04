#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "quickjs.h"
#include "hyperlight_guest.h"

// evaluate the given raw javascript expression and return the result as a string
const char *guest_function(const char *code) {
    JSRuntime *rt;
    JSContext *ctx;
    JSValue result;
    const char *st;

    rt = JS_NewRuntime();
    ctx = JS_NewContext(rt);

    // Evaluate the JavaScript expression
    result = JS_Eval(ctx, code, strlen(code), "<eval>", JS_EVAL_TYPE_GLOBAL);

    // Check for errors
    if (JS_IsException(result)) {
        JSValue exception = JS_GetException(ctx);
        st = JS_ToCString(ctx, exception);
        JS_FreeValue(ctx, exception);
    } else {
        st = JS_ToCString(ctx, result);
    }

    JS_FreeValue(ctx, result);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);

    return st;
}

HYPERLIGHT_WRAP_FUNCTION(guest_function, String, 1, String);

void hyperlight_main(void) {
    HYPERLIGHT_REGISTER_FUNCTION("EvalScript", guest_function);
}

// This dispatch function is only used when the host dispatches a guest function
// call but there is no registered guest function with the given name.
hl_Vec *c_guest_dispatch_function(const hl_FunctionCall *function_call) {
    return NULL;
}