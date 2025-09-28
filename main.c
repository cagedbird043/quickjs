// 文件: quickjs/main.c

#include <stdio.h>
#include <string.h>
#include "quickjs.h"
#include "quickjs-libc.h"

int main(int argc, char **argv)
{
    JSRuntime *rt;
    JSContext *ctx;
    JSValue val;

    rt = JS_NewRuntime();
    if (!rt)
    {
        fprintf(stderr, "Failed to create JSRuntime\n");
        return 1;
    }

    ctx = JS_NewContext(rt);
    if (!ctx)
    {
        fprintf(stderr, "Failed to create JSContext\n");
        JS_FreeRuntime(rt);
        return 1;
    }

    js_std_add_helpers(ctx, argc, argv);

    const char *script = "let a = 5; let b = 10; console.log('Hello from QuickJS!', a + b); a + b;";
    printf("--- Evaluating script ---\n");
    val = JS_Eval(ctx, script, strlen(script), "<eval>", JS_EVAL_TYPE_GLOBAL);

    if (JS_IsException(val))
    {
        js_std_dump_error(ctx);
        JS_FreeValue(ctx, val);
    }
    else
    {
        int result;
        if (JS_ToInt32(ctx, &result, val) == 0)
        {
            printf("--- Script evaluated successfully ---\n");
            printf("Result: %d\n", result);
        }
        JS_FreeValue(ctx, val);
    }

    /*
     * 对于同步脚本，在 JS_Eval 之后不需要事件循环。
     * 直接清理资源即可。
     * js_std_loop(ctx); // <--- 已移除
     */

    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);

    printf("--- Cleanup successful. Exiting. ---\n"); // 加一句输出来确认

    return 0;
}