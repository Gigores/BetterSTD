#include "stdio.h"
#include "btrstd/string_view.h"
#include "assert.h"
#include "string.h"

// test `StringView_fromCString` and `StringView_cropLeft`
static void test1(void)
{
    printf("> test1\n");

    const char *TEXT = "AЖ你😀";

    btr_string_view_s view = BTR_StringView_fromCString(TEXT);

    view = BTR_StringView_cropLeft(view, 1);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
            .data = "Ж你😀",
            .length = strlen("Ж你😀")
        }
    ) == 0);

    view = BTR_StringView_cropLeft(view, 1);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
            .data = "你😀",
            .length = strlen("你😀")
        }
    ) == 0);

    view = BTR_StringView_cropLeft(view, 1);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
            .data = "😀",
            .length = strlen("😀")
        }
    ) == 0);

    view = BTR_StringView_cropLeft(view, 1);
    assert(view.length == 0);
}

// test `cropRight`
static void test3(void)
{
    printf("> test3\n");

    const char *TEXT = "😀你ЖA";

    btr_string_view_s view = BTR_StringView_fromCString(TEXT);

    view = BTR_StringView_cropRight(view, 1);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
            .data = "😀你Ж",
            .length = strlen("😀你Ж")
        }
    ) == 0);

    view = BTR_StringView_cropRight(view, 1);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
            .data = "😀你",
            .length = strlen("😀你")
        }
    ) == 0);

    view = BTR_StringView_cropRight(view, 1);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
            .data = "😀",
            .length = strlen("😀")
        }
    ) == 0);

    view = BTR_StringView_cropRight(view, 1);
    assert(view.length == 0);
}
// test `cropLeft` and `cropRight` at the same time
static void test5(void)
{
    printf("> test5\n");

    const char *TEXT = "AЖ你😀BŁ文🚀";

    btr_string_view_s view = BTR_StringView_fromCString(TEXT);

    view = BTR_StringView_cropRight(view, 1);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
            .data = "AЖ你😀BŁ文",
            .length = strlen("AЖ你😀BŁ文")
        }
    ) == 0);

    view = BTR_StringView_cropLeft(view, 2);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
            .data = "你😀BŁ文",
            .length = strlen("你😀BŁ文")
        }
    ) == 0);

    view = BTR_StringView_cropRight(view, 2);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
            .data = "你😀B",
            .length = strlen("你😀B")
        }
    ) == 0);

    view = BTR_StringView_cropLeft(view, 2);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
            .data = "B",
            .length = strlen("B")
        }
    ) == 0);

    view = BTR_StringView_cropRight(view, 1);
    assert(view.length == 0);
}
// test `revertRight` and `revertLeft`
static void test7(void)
{
    printf("> test7\n");

    const char *TEXT = "AЖ你😀BŁ文🚀";

    btr_string_view_s view = BTR_StringView_fromCString(TEXT);

    view = BTR_StringView_cropRight(view, 1);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
            .data = "AЖ你😀BŁ文",
            .length = strlen("AЖ你😀BŁ文")
        }
    ) == 0);

    view = BTR_StringView_revertRight(view, 1);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
            .data = "AЖ你😀BŁ文🚀",
            .length = strlen("AЖ你😀BŁ文🚀")
        }
    ) == 0);

    view = BTR_StringView_cropLeft(view, 1);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
            .data = "Ж你😀BŁ文🚀",
            .length = strlen("Ж你😀BŁ文🚀")
        }
    ) == 0);

    view = BTR_StringView_revertLeft(view, 1);
    assert(BTR_StringView_compare(
        view,
        (btr_string_view_s) {
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

    btr_string_view_s view = BTR_StringView_fromCString(TEXT);

    assert(BTR_StringView_byteCount(view) == strlen(TEXT));
    assert(BTR_StringView_len(view) == strlen(TEXT) / 2);
    assert(!BTR_StringView_isEmpty(view));

    view = BTR_StringView_cropLeft(view, 2);

    assert(BTR_StringView_byteCount(view) == strlen(TEXT) - 4);
    assert(BTR_StringView_len(view) == (strlen(TEXT) - 4) / 2);
    assert(!BTR_StringView_isEmpty(view));

    for (size_t i = 0; i < BTR_StringView_len(view); i++)
        assert(*BTR_StringView_charAt(view, i) == TEXT[4 + i * 2]);

    size_t len = strlen(TEXT);
    for (size_t i = 1; i < BTR_StringView_len(view); i++)
        assert(*BTR_StringView_charAt(view, -i) == TEXT[len - i * 2]);

    btr_string_view_s prefix1 = BTR_StringView_fromCString("σκεπ");
    assert(BTR_StringView_startsWith(view, prefix1));

    btr_string_view_s prefix2 = BTR_StringView_fromCString("οφθό");
    assert(!BTR_StringView_startsWith(view, prefix2));

    assert(BTR_StringView_endsWith(view, "γμία"));

    assert(!BTR_StringView_endsWith(view, "οφθό"));

    assert(BTR_StringView_find(view, "οφθό").data);

    assert(!BTR_StringView_find(view, "hello, great sir!").data);

    btr_string_view_s sub = BTR_StringView_substring(view, 2, 8);
    btr_string_view_s testView = BTR_StringView_fromCString("επάζωτην");
    assert(!BTR_StringView_compare(sub, testView));
}
// test `trim`
static void test9(void)
{
    printf("> test9\n");

    const char *TEXT = "   я не знаю что значит этот текст      ";

    btr_string_view_s view = BTR_StringView_fromCString(TEXT);

    view = BTR_StringView_trim(view);
    btr_string_view_s testView = BTR_StringView_fromCString("я не знаю что значит этот текст");
    assert(!BTR_StringView_compare(view, testView));
}

// test `trimLeftTo`, `trimLeftPast`, `trimRightTo`, `trimRightPast`
static void test10(void)
{
    printf("> test10\n");

    const char *TEXT = "image.png";

    btr_string_view_s view = BTR_StringView_fromCString(TEXT);

    btr_string_view_s leftTo = BTR_StringView_trimLeftTo(view, ".");
    btr_string_view_s testLeftTo = BTR_StringView_fromCString(".png");
    assert(!BTR_StringView_compare(leftTo, testLeftTo));

    btr_string_view_s leftPast = BTR_StringView_trimLeftPast(view, ".");
    btr_string_view_s testLeftPast = BTR_StringView_fromCString("png");
    assert(!BTR_StringView_compare(leftPast, testLeftPast));

    btr_string_view_s rightTo = BTR_StringView_trimRightTo(view, ".");
    btr_string_view_s testRightTo = BTR_StringView_fromCString("image.");
    assert(!BTR_StringView_compare(rightTo, testRightTo));

    btr_string_view_s rightPast = BTR_StringView_trimRightPast(view, ".");
    btr_string_view_s testRightPast = BTR_StringView_fromCString("image");
    assert(!BTR_StringView_compare(rightPast, testRightPast));
}

// test trim[Left/Right][To/Past] with non-existent character (should be no-op)
static void test11(void)
{
    printf("> test11\n");

    const char *TEXT = "hello world";

    btr_string_view_s view = BTR_StringView_fromCString(TEXT);

    btr_string_view_s leftTo = BTR_StringView_trimLeftTo(view, ".");
    assert(!BTR_StringView_compare(leftTo, view));

    btr_string_view_s leftPast = BTR_StringView_trimLeftPast(view, ".");
    assert(!BTR_StringView_compare(leftPast, view));

    btr_string_view_s rightTo = BTR_StringView_trimRightTo(view, ".");
    assert(!BTR_StringView_compare(rightTo, view));

    btr_string_view_s rightPast = BTR_StringView_trimRightPast(view, ".");
    assert(!BTR_StringView_compare(rightPast, view));
}

// test trim[Left/Right][To/Past] with multi-byte UTF-8 character
static void test12(void)
{
    printf("> test12\n");

    const char *TEXT = "AЖ你😀BŁ文🚀";

    btr_string_view_s view = BTR_StringView_fromCString(TEXT);

    btr_string_view_s leftTo = BTR_StringView_trimLeftTo(view, "😀");
    btr_string_view_s testLeftTo = BTR_StringView_fromCString("😀BŁ文🚀");
    assert(!BTR_StringView_compare(leftTo, testLeftTo));

    btr_string_view_s leftPast = BTR_StringView_trimLeftPast(view, "😀");
    btr_string_view_s testLeftPast = BTR_StringView_fromCString("BŁ文🚀");
    assert(!BTR_StringView_compare(leftPast, testLeftPast));

    btr_string_view_s rightTo = BTR_StringView_trimRightTo(view, "Ł");
    btr_string_view_s testRightTo = BTR_StringView_fromCString("AЖ你😀BŁ");
    assert(!BTR_StringView_compare(rightTo, testRightTo));

    btr_string_view_s rightPast = BTR_StringView_trimRightPast(view, "Ł");
    btr_string_view_s testRightPast = BTR_StringView_fromCString("AЖ你😀B");
    assert(!BTR_StringView_compare(rightPast, testRightPast));
}

int main(void) {
    test1();
    test3();
    test5();
    test7();
    test8();
    test9();
    test10();
    test11();
    test12();
    printf("SUCCESS\n");
}
