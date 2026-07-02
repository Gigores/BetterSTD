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
    printf("SUCCESS\n");
}
