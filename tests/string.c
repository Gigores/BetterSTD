#include "stdio.h"
#include "btrstd/bstring.h"


void test1(void)
{

    const char *TEST_STRING = "Չին ֆիզիկոսը օճառաջուր ցողելով բժշկում է հայ գնդապետի փքված ձախ թևը։";
    btr_string_t str = BTR_String_view(TEST_STRING);
    BTR_String_cropLeft(&str, 2);
    printf("%d\n", str.start);
    printf("Print test > "BTR_STRING_FORMAT"\n", BTR_STRING_ARGS(str));
    BTR_String_free(&str);
}

int main(void) {
    test1();
}
