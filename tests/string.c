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

int main(void) {
    test1();
    test2();
}
