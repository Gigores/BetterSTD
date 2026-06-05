#include "stdio.h"
#include "btrstd/string_view.h"
#include "assert.h"
#include "string.h"

// test `StringView_fromCString` and `StringView_cropLeft`
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

// test `cropRight`
static void test3(void)
{
    printf("> test3\n");

    const char *TEXT = "😀你ЖA";

    btr_string_view_t view = BTR_StringView_fromCString(TEXT);

    BTR_StringView_cropRight(&view, 1);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "😀你Ж",
            .length = strlen("😀你Ж")
        }
    ) == 0);

    BTR_StringView_cropRight(&view, 1);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "😀你",
            .length = strlen("😀你")
        }
    ) == 0);

    BTR_StringView_cropRight(&view, 1);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "😀",
            .length = strlen("😀")
        }
    ) == 0);

    BTR_StringView_cropRight(&view, 1);
    assert(view.length == 0);
}
// test `cropLeft` and `cropRight` at the same time
static void test5(void)
{
    printf("> test5\n");

    const char *TEXT = "AЖ你😀BŁ文🚀";

    btr_string_view_t view = BTR_StringView_fromCString(TEXT);

    BTR_StringView_cropRight(&view, 1);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "AЖ你😀BŁ文",
            .length = strlen("AЖ你😀BŁ文")
        }
    ) == 0);

    BTR_StringView_cropLeft(&view, 2);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "你😀BŁ文",
            .length = strlen("你😀BŁ文")
        }
    ) == 0);

    BTR_StringView_cropRight(&view, 2);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "你😀B",
            .length = strlen("你😀B")
        }
    ) == 0);

    BTR_StringView_cropLeft(&view, 2);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "B",
            .length = strlen("B")
        }
    ) == 0);

    BTR_StringView_cropRight(&view, 1);
    assert(view.length == 0);
}
// test `revertRight` and `revertLeft`
static void test7(void)
{
    printf("> test7\n");

    const char *TEXT = "AЖ你😀BŁ文🚀";

    btr_string_view_t view = BTR_StringView_fromCString(TEXT);

    BTR_StringView_cropRight(&view, 1);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "AЖ你😀BŁ文",
            .length = strlen("AЖ你😀BŁ文")
        }
    ) == 0);

    BTR_StringView_revertRight(&view, 1);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "AЖ你😀BŁ文🚀",
            .length = strlen("AЖ你😀BŁ文🚀")
        }
    ) == 0);

    BTR_StringView_cropLeft(&view, 1);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "Ж你😀BŁ文🚀",
            .length = strlen("Ж你😀BŁ文🚀")
        }
    ) == 0);

    BTR_StringView_revertLeft(&view, 1);
    assert(BTR_StringView_compare(
        &view,
        &(btr_string_view_t) {
            .data = "AЖ你😀BŁ文🚀",
            .length = strlen("AЖ你😀BŁ文🚀")
        }
    ) == 0);
}
// test a bunch of shit
static void test8(void)
{
    printf("> test8\n");

    const char *TEXT = "Ξεσκεπάζωτηνψυχοφθόραβδελυγμία";

    btr_string_view_t view = BTR_StringView_fromCString(TEXT);

    assert(BTR_StringView_byteCount(&view) == strlen(TEXT));
    assert(BTR_StringView_len(&view) == strlen(TEXT) / 2);
    assert(!BTR_StringView_isEmpty(&view));

    BTR_StringView_cropLeft(&view, 2);

    assert(BTR_StringView_byteCount(&view) == strlen(TEXT) - 4);
    assert(BTR_StringView_len(&view) == (strlen(TEXT) - 4) / 2);
    assert(!BTR_StringView_isEmpty(&view));

    for (size_t i = 0; i < BTR_StringView_len(&view); i++)
        assert(*BTR_StringView_charAt(&view, i) == TEXT[4 + i * 2]);

    size_t len = strlen(TEXT);
    for (size_t i = 1; i < BTR_StringView_len(&view); i++)
        assert(*BTR_StringView_charAt(&view, -i) == TEXT[len - i * 2]);

    btr_string_view_t prefix1 = BTR_StringView_fromCString("σκεπ");
    assert(BTR_StringView_startsWith(&view, &prefix1));

    btr_string_view_t prefix2 = BTR_StringView_fromCString("οφθό");
    assert(!BTR_StringView_startsWith(&view, &prefix2));

    assert(BTR_StringView_endsWith(&view, "γμία"));

    assert(!BTR_StringView_endsWith(&view, "οφθό"));

    assert(BTR_StringView_find(&view, "οφθό").data);

    assert(!BTR_StringView_find(&view, "hello, great sir!").data);

    btr_string_view_t sub = BTR_StringView_substring(&view, 2, 8);
    btr_string_view_t testView = BTR_StringView_fromCString("επάζωτην");
    assert(!BTR_StringView_compare(&sub, &testView));
}
// test `trim`
static void test9(void)
{
    printf("> test9\n");

    const char *TEXT = "   я не знаю что значит этот текст      ";

    btr_string_view_t view = BTR_StringView_fromCString(TEXT);

    BTR_StringView_trim(&view);
    btr_string_view_t testView = BTR_StringView_fromCString("я не знаю что значит этот текст");
    assert(!BTR_StringView_compare(&view, &testView));
}

int main(void) {
    test1();
    test3();
    test5();
    test7();
    test8();
    test9();
    printf("SUCCESS\n");
}
