#include "btrstd/string.h"
#include "assert.h"
#include "stdio.h"
#include <string.h>

// test fromCString and compare
static void test1(void)
{
    printf("> test1\n");
    btr_string_s string = BTR_String_from("Test string", NULL);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("Test string")) == 0);
    BTR_String_free(&string);
}
// test toCString
static void test2(void)
{
    printf("> test2\n");
    btr_string_view_s sv = BTR_StringView_fromCString("Test string");
    btr_string_s string = BTR_String_fromStringView(sv, NULL);
    const char *cstr = BTR_String_toCString(&string, NULL);
    assert(strcmp(cstr, "Test string") == 0);
    BTR_String_free(&string);
    BTR_Allocator_deallocate(string.data.allocator, (void *)cstr);
}
// test append
static void test3(void)
{
    printf("> test3\n");
    btr_string_s string = BTR_String_from("Тестовая строка", NULL);
    BTR_String_append(&string, " номер 12");
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("Тестовая строка номер 12")) == 0);
    BTR_String_free(&string);
}
// test prepend
static void test4(void)
{
    printf("> test4\n");
    btr_string_s string = BTR_String_from("Тестовая строка", NULL);
    BTR_String_prepend(&string, "42 ");
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("42 Тестовая строка")) == 0);
    BTR_String_free(&string);
}
// test crop
static void test5(void)
{
    printf("> test5\n");
    btr_string_s string = BTR_String_from("Тестовая строка двенадцатая", NULL);
    BTR_String_cropLeft(&string, 9);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("строка двенадцатая")) == 0);
    BTR_String_cropRight(&string, 12);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("строка")) == 0);
    BTR_String_free(&string);
}
// test pop
static void test6(void)
{
    printf("> test6\n");
    btr_string_s string = BTR_String_from("Test string", NULL);
    char buffer = '\0';
    BTR_String_pop(&string, 1, &buffer);
    assert(buffer == 'e');
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("Tst string")) == 0);
    BTR_String_free(&string);

    btr_string_s string2 = BTR_String_from("Тестовая строка двенадцатая", NULL);
    BTR_String_pop(&string2, 3, NULL);
    assert(BTR_String_compareView(&string2, BTR_StringView_fromCString("Тесовая строка двенадцатая")) == 0);
    BTR_String_free(&string2);
    
    btr_string_s string3 = BTR_String_from("a🔥b", NULL);
    char buffer2 = '\0';
    BTR_String_pop(&string3, 2, &buffer2);
    assert(buffer2 == 'b');
    assert(BTR_String_compareView(&string3, BTR_StringView_fromCString("a🔥")) == 0);
    BTR_String_free(&string3);
}

// test reserve
static void test7(void)
{
    printf("> test7\n");
    btr_string_s string = BTR_String_from("Hello", NULL);

    // reserve larger — should grow
    BTR_String_reserve(&string, 128);
    assert(string.data.capacity >= 128);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("Hello")) == 0);

    // reserve smaller — should be no-op
    BTR_String_reserve(&string, 2);
    assert(string.data.capacity >= 128);

    BTR_String_free(&string);
}
// test reserveNew
static void test8(void)
{
    printf("> test8\n");
    btr_string_s string = BTR_String_from("Hello", NULL);

    BTR_String_reserveNew(&string, 50);
    assert(string.data.capacity >= string.data.count + 50);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("Hello")) == 0);

    BTR_String_free(&string);
}
// test cropCapacity
static void test9(void)
{
    printf("> test9\n");
    btr_string_s string = BTR_String_from("Hello, world!", NULL);

    // grow capacity first so we can verify crop
    BTR_String_reserve(&string, 256);
    assert(string.data.capacity >= 256);

    BTR_String_cropCapacity(&string);
    assert(string.data.capacity == string.data.count);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("Hello, world!")) == 0);

    BTR_String_free(&string);
}
// test insert
static void test10(void)
{
    printf("> test10\n");
    btr_string_s string = BTR_String_from("Привет, мир!", NULL);
    BTR_String_insertCString(&string, " мой милый", 7);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("Привет, мой милый мир!")) == 0);
    BTR_String_free(&string);

    btr_string_s string2 = BTR_String_from("Hello, World!", NULL);
    BTR_String_insertCString(&string2, " my pretty", 6);
    assert(BTR_String_compareView(&string2, BTR_StringView_fromCString("Hello, my pretty World!")) == 0);
    BTR_String_insertChar(&string2, '!', 23);
    assert(BTR_String_compareView(&string2, BTR_StringView_fromCString("Hello, my pretty World!!")) == 0);
    BTR_String_free(&string2);
}

// test remove (UTF-8 characters)
static void test11(void)
{
    printf("> test11\n");
    btr_string_s string = BTR_String_from("Hello World!", NULL);
    BTR_String_remove(&string, 6, 6);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("Hello ")) == 0);
    BTR_String_free(&string);

    btr_string_s string2 = BTR_String_from("a🔥b", NULL);
    BTR_String_remove(&string2, 1, 1);
    assert(BTR_String_compareView(&string2, BTR_StringView_fromCString("ab")) == 0);
    BTR_String_free(&string2);

    btr_string_s string3 = BTR_String_from("Hello", NULL);
    BTR_String_remove(&string3, 0, 2);
    assert(BTR_String_compareView(&string3, BTR_StringView_fromCString("llo")) == 0);
    BTR_String_free(&string3);
}
// test removeBytes (raw bytes)
static void test12(void)
{
    printf("> test12\n");
    btr_string_s string = BTR_String_from("Hello World!", NULL);
    BTR_String_removeBytes(&string, 6, 6);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("Hello ")) == 0);
    BTR_String_free(&string);

    btr_string_s string2 = BTR_String_from("a🔥b", NULL);
    BTR_String_removeBytes(&string2, 1, 4);
    assert(BTR_String_compareView(&string2, BTR_StringView_fromCString("ab")) == 0);
    BTR_String_free(&string2);

    btr_string_s string3 = BTR_String_from("Hello", NULL);
    BTR_String_removeBytes(&string3, 0, 3);
    assert(BTR_String_compareView(&string3, BTR_StringView_fromCString("lo")) == 0);
    BTR_String_free(&string3);
}
// test clear
static void test13(void)
{
    printf("> test13\n");
    btr_string_s string = BTR_String_from("Hello", NULL);
    size_t len = BTR_String_len(&string);
    assert(len == 5);
    BTR_String_clear(&string);
    assert(BTR_String_len(&string) == 0);
    BTR_String_free(&string);

    btr_string_s string2 = BTR_String_from("", NULL);
    assert(BTR_String_isEmpty(&string2));
    BTR_String_clear(&string2);
    assert(BTR_String_isEmpty(&string2));
    BTR_String_free(&string2);
}

// test replaceCString
static void test14(void)
{
    printf("> test14\n");
    btr_string_s string = BTR_String_from("Hello World, Hello!", NULL);
    size_t count = BTR_String_replaceCString(&string, "Hello", "Hi");
    assert(count == 2);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("Hi World, Hi!")) == 0);
    BTR_String_free(&string);

    // no match
    btr_string_s string2 = BTR_String_from("Hello World", NULL);
    count = BTR_String_replaceCString(&string2, "xyz", "abc");
    assert(count == 0);
    assert(BTR_String_compareView(&string2, BTR_StringView_fromCString("Hello World")) == 0);
    BTR_String_free(&string2);

    // replace with longer string
    btr_string_s string3 = BTR_String_from("a b a b a", NULL);
    count = BTR_String_replaceCString(&string3, "a", "abc");
    assert(count == 3);
    assert(BTR_String_compareView(&string3, BTR_StringView_fromCString("abc b abc b abc")) == 0);
    BTR_String_free(&string3);

    // replace with empty string (deletion)
    btr_string_s string4 = BTR_String_from("Hello World", NULL);
    count = BTR_String_replaceCString(&string4, "l", "");
    assert(count == 3);
    assert(BTR_String_compareView(&string4, BTR_StringView_fromCString("Heo Word")) == 0);
    BTR_String_free(&string4);

    // replace empty with non-empty (insertion at every position)
    btr_string_s string5 = BTR_String_from("abc", NULL);
    count = BTR_String_replaceCString(&string5, "", "X");
    assert(count == 0);
    assert(BTR_String_compareView(&string5, BTR_StringView_fromCString("abc")) == 0);
    BTR_String_free(&string5);
}
// test replaceString
static void test15(void)
{
    printf("> test15\n");
    btr_string_s string = BTR_String_from("foo bar foo baz foo", NULL);
    btr_string_s from = BTR_String_from("foo", NULL);
    btr_string_s to = BTR_String_from("qux", NULL);
    size_t count = BTR_String_replaceString(&string, from, to);
    assert(count == 3);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("qux bar qux baz qux")) == 0);
    BTR_String_free(&string);
    BTR_String_free(&from);
    BTR_String_free(&to);

    // non-ASCII
    btr_string_s string2 = BTR_String_from("Тест Тест Тест", NULL);
    btr_string_s from2 = BTR_String_from("Тест", NULL);
    btr_string_s to2 = BTR_String_from("Модуль", NULL);
    count = BTR_String_replaceString(&string2, from2, to2);
    assert(count == 3);
    assert(BTR_String_compareView(&string2, BTR_StringView_fromCString("Модуль Модуль Модуль")) == 0);
    BTR_String_free(&string2);
    BTR_String_free(&from2);
    BTR_String_free(&to2);
}
// test replaceStringView
static void test16(void)
{
    printf("> test16\n");
    btr_string_s string = BTR_String_from("aaa bbb aaa", NULL);
    btr_string_view_s from = BTR_StringView_fromCString("aaa");
    btr_string_view_s to = BTR_StringView_fromCString("ccc");
    size_t count = BTR_String_replaceStringView(&string, from, to);
    assert(count == 2);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("ccc bbb ccc")) == 0);
    BTR_String_free(&string);

    // empty replacement
    btr_string_s string2 = BTR_String_from("aaa bbb aaa", NULL);
    btr_string_view_s from2 = BTR_StringView_fromCString("aaa");
    btr_string_view_s to2 = BTR_StringView_fromCString("");
    count = BTR_String_replaceStringView(&string2, from2, to2);
    assert(count == 2);
    assert(BTR_String_compareView(&string2, BTR_StringView_fromCString(" bbb ")) == 0);
    BTR_String_free(&string2);
}
// test toUpper
static void test17(void)
{
    printf("> test17\n");
    btr_string_s string = BTR_String_from("Hello, World!", NULL);
    BTR_String_toUpper(&string);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("HELLO, WORLD!")) == 0);
    BTR_String_free(&string);

    // already uppercase
    btr_string_s string2 = BTR_String_from("ABC", NULL);
    BTR_String_toUpper(&string2);
    assert(BTR_String_compareView(&string2, BTR_StringView_fromCString("ABC")) == 0);
    BTR_String_free(&string2);

    // mixed with non-ASCII (should be left untouched)
    btr_string_s string3 = BTR_String_from("Привет hello", NULL);
    BTR_String_toUpper(&string3);
    assert(BTR_String_compareView(&string3, BTR_StringView_fromCString("Привет HELLO")) == 0);
    BTR_String_free(&string3);
}
// test toLower
static void test18(void)
{
    printf("> test18\n");
    btr_string_s string = BTR_String_from("Hello, World!", NULL);
    BTR_String_toLower(&string);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("hello, world!")) == 0);
    BTR_String_free(&string);

    // already lowercase
    btr_string_s string2 = BTR_String_from("abc", NULL);
    BTR_String_toLower(&string2);
    assert(BTR_String_compareView(&string2, BTR_StringView_fromCString("abc")) == 0);
    BTR_String_free(&string2);

    // mixed with non-ASCII
    btr_string_s string3 = BTR_String_from("ПРИВЕТ HELLO", NULL);
    BTR_String_toLower(&string3);
    assert(BTR_String_compareView(&string3, BTR_StringView_fromCString("ПРИВЕТ hello")) == 0);
    BTR_String_free(&string3);
}

int main(void)
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    test10();
    test11();
    test12();
    test13();
    test14();
    test15();
    test16();
    test17();
    test18();
    printf("SUCCESS\n");
}
