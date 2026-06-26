#include "btrstd/string.h"
#include "assert.h"
#include "stdio.h"

static void test1(void)
{
    printf("> test1\n");
    btr_string_s string = BTR_String_fromCString("Test string", NULL);
    assert(BTR_String_compareView(&string, BTR_StringView_fromCString("Test string")) == 0);
    BTR_String_free(&string);
}

int main(void)
{
    test1();
    printf("SUCCESS\n");
}
