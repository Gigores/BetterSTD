#include "btrstd/string.h"
#include "assert.h"
#include "stdio.h"
#include <string.h>

// test fromCString and compare
static void test1(void)
{
    printf("> test1\n");
    btr_string_s string = BTR_String_fromCString("Test string", NULL);
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

int main(void)
{
    test1();
    test2();
    printf("SUCCESS\n");
}
