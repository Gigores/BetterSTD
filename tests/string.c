#include "stdio.h"
#include "btrstd/bstring.h"
#include "assert.h"
#include "string.h"

// test `BTR_StringView_fromCString` and `BTR_StringView_cropLeft`
static void test1(void)
{
    printf("> test1\n");

    const char *TEXT = "AЖ你😀";

    btr_string_view_t view = BTR_StringView_fromCString(TEXT);

    BTR_StringView_cropLeft(&view, 1);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "Ж你😀",
            .length = strlen("Ж你😀")
        }
    ) == 0);

    BTR_StringView_cropLeft(&view, 1);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "你😀",
            .length = strlen("你😀")
        }
    ) == 0);

    BTR_StringView_cropLeft(&view, 1);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "😀",
            .length = strlen("😀")
        }
    ) == 0);

    BTR_StringView_cropLeft(&view, 1);
    assert(view.length == 0);
}

// test `BTR_String_fromCString` and `BTR_String_cropLeft`
static void test2(void)
{
    printf("> test2\n");

    const char *TEXT = "AЖ你😀";

    btr_string_t view = BTR_String_clone(TEXT);

    BTR_String_cropLeft(&view, 1);
    assert(BTR_String_compareWithView(
        &view,
        &(btr_string_view_t) {
            .data = "Ж你😀",
            .length = strlen("Ж你😀")
        }
    ) == 0);

    BTR_String_cropLeft(&view, 1);
    assert(BTR_String_compareWithView(
        &view,
        &(btr_string_view_t) {
            .data = "你😀",
            .length = strlen("你😀")
        }
    ) == 0);

    BTR_String_cropLeft(&view, 1);
    assert(BTR_String_compareWithView(
        &view,
        &(btr_string_view_t) {
            .data = "😀",
            .length = strlen("😀")
        }
    ) == 0);

    BTR_String_cropLeft(&view, 1);
    assert(view.length == 0);

    BTR_String_free(&view);
}

int main(void) {
    test1();
    test2();
}
