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

int main(void)
{
    test1();
    test2();
    test3();
    printf("SUCCESS\n");
}
