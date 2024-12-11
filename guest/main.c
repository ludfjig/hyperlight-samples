#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "quickjs.h"

#ifdef HYPERLIGHT
#include "hyperlight_guest.h"
#else
#include <stdlib.h>
#endif 

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

#ifdef HYPERLIGHT
HYPERLIGHT_WRAP_FUNCTION(guest_function, String, 1, String);

void hyperlight_main(void) {
    HYPERLIGHT_REGISTER_FUNCTION("EvalScript", guest_function);
}

// This dispatch function is only used when the host dispatches a guest function
// call but there is no registered guest function with the given name.
hl_Vec *c_guest_dispatch_function(const hl_FunctionCall *function_call) {
    return NULL;
}
#else
// If you want to run this code outside of hyperlight, you can use the following main function
int main(int argc, char* argv[]) {
    char* buffer = NULL;
    size_t len = 0;
    const char * res;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return 1;
    }
    char* file_path = argv[1];
    FILE * fp = fopen(file_path, "rb");
    ssize_t bytes_read = getdelim(&buffer, &len, '\0', fp);

    res = guest_function(buffer);
    printf("%s\n", res);
    free(buffer);
    fclose(fp);
    return 0;
}
#endif
