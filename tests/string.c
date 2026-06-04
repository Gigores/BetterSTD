#include "stdio.h"
#include "btrstd/bstring.h"
#include "assert.h"
#include "string.h"

// test `StringView_fromCString` and `StringView_cropLeft`
static void test1(void)
{
    printf("> test1\n");

    const char *TEXT = "AЖ你😀";

    string_view_t view = StringView_fromCString(TEXT);

    StringView_cropLeft(&view, 1);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "Ж你😀",
            .length = strlen("Ж你😀")
        }
    ) == 0);

    StringView_cropLeft(&view, 1);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "你😀",
            .length = strlen("你😀")
        }
    ) == 0);

    StringView_cropLeft(&view, 1);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "😀",
            .length = strlen("😀")
        }
    ) == 0);

    StringView_cropLeft(&view, 1);
    assert(view.length == 0);
}

// test `String_fromCString` and `String_cropLeft`
static void test2(void)
{
    printf("> test2\n");

    const char *TEXT = "AЖ你😀";

    string_t view = String_clone(TEXT);

    String_cropLeft(&view, 1);
    assert(String_compareView(
        &view,
        &(string_view_t) {
            .data = "Ж你😀",
            .length = strlen("Ж你😀")
        }
    ) == 0);

    String_cropLeft(&view, 1);
    assert(String_compareView(
        &view,
        &(string_view_t) {
            .data = "你😀",
            .length = strlen("你😀")
        }
    ) == 0);

    String_cropLeft(&view, 1);
    assert(String_compareView(
        &view,
        &(string_view_t) {
            .data = "😀",
            .length = strlen("😀")
        }
    ) == 0);

    String_cropLeft(&view, 1);
    assert(view.length == 0);

    String_free(&view);
}
// test `cropRight`
static void test3(void)
{
    printf("> test3\n");

    const char *TEXT = "😀你ЖA";

    string_view_t view = StringView_fromCString(TEXT);

    StringView_cropRight(&view, 1);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "😀你Ж",
            .length = strlen("😀你Ж")
        }
    ) == 0);

    StringView_cropRight(&view, 1);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "😀你",
            .length = strlen("😀你")
        }
    ) == 0);

    StringView_cropRight(&view, 1);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "😀",
            .length = strlen("😀")
        }
    ) == 0);

    StringView_cropRight(&view, 1);
    assert(view.length == 0);
}
// test `cropRight`
static void test4(void)
{
    printf("> test4\n");

    const char *TEXT = "😀你ЖA";

    string_t view = String_clone(TEXT);

    String_cropRight(&view, 1);
    assert(String_compareView(
        &view,
        &(string_view_t) {
            .data = "😀你Ж",
            .length = strlen("😀你Ж")
        }
    ) == 0);

    String_cropRight(&view, 1);
    assert(String_compareView(
        &view,
        &(string_view_t) {
            .data = "😀你",
            .length = strlen("😀你")
        }
    ) == 0);

    String_cropRight(&view, 1);
    assert(String_compareView(
        &view,
        &(string_view_t) {
            .data = "😀",
            .length = strlen("😀")
        }
    ) == 0);

    String_cropRight(&view, 1);
    assert(view.length == 0);

    String_free(&view);
}
// test `cropLeft` and `cropRight` at the same time
static void test5(void)
{
    printf("> test5\n");

    const char *TEXT = "AЖ你😀BŁ文🚀";

    string_view_t view = StringView_fromCString(TEXT);

    StringView_cropRight(&view, 1);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "AЖ你😀BŁ文",
            .length = strlen("AЖ你😀BŁ文")
        }
    ) == 0);

    StringView_cropLeft(&view, 2);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "你😀BŁ文",
            .length = strlen("你😀BŁ文")
        }
    ) == 0);

    StringView_cropRight(&view, 2);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "你😀B",
            .length = strlen("你😀B")
        }
    ) == 0);

    StringView_cropLeft(&view, 2);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "B",
            .length = strlen("B")
        }
    ) == 0);

    StringView_cropRight(&view, 1);
    assert(view.length == 0);
}
// test `cropLeft` and `cropRight` at the same time
static void test6(void)
{
    printf("> test6\n");

    const char *TEXT = "AЖ你😀BŁ文🚀";

    string_t view = String_clone(TEXT);

    String_cropRight(&view, 1);
    assert(String_compareView(
        &view,
        &(string_view_t) {
            .data = "AЖ你😀BŁ文",
            .length = strlen("AЖ你😀BŁ文")
        }
    ) == 0);

    String_cropLeft(&view, 2);
    assert(String_compareView(
        &view,
        &(string_view_t) {
            .data = "你😀BŁ文",
            .length = strlen("你😀BŁ文")
        }
    ) == 0);

    String_cropRight(&view, 2);
    assert(String_compareView(
        &view,
        &(string_view_t) {
            .data = "你😀B",
            .length = strlen("你😀B")
        }
    ) == 0);

    String_cropLeft(&view, 2);
    assert(String_compareView(
        &view,
        &(string_view_t) {
            .data = "B",
            .length = strlen("B")
        }
    ) == 0);

    String_cropRight(&view, 1);
    assert(view.length == 0);

    String_free(&view);
}
// test `revertRight` and `revertLeft`
static void test7(void)
{
    printf("> test7\n");

    const char *TEXT = "AЖ你😀BŁ文🚀";

    string_view_t view = StringView_fromCString(TEXT);

    StringView_cropRight(&view, 1);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "AЖ你😀BŁ文",
            .length = strlen("AЖ你😀BŁ文")
        }
    ) == 0);

    StringView_revertRight(&view, 1);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "AЖ你😀BŁ文🚀",
            .length = strlen("AЖ你😀BŁ文🚀")
        }
    ) == 0);

    StringView_cropLeft(&view, 1);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "Ж你😀BŁ文🚀",
            .length = strlen("Ж你😀BŁ文🚀")
        }
    ) == 0);

    StringView_revertLeft(&view, 1);
    assert(StringView_compare(
        &view,
        &(string_view_t) {
            .data = "AЖ你😀BŁ文🚀",
            .length = strlen("AЖ你😀BŁ文🚀")
        }
    ) == 0);
}

int main(void) {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    printf("SUCCESS\n");
}
