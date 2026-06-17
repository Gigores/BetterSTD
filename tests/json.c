#include "btrstd/json.h"

int main(void)
{
    BTR_jsonDeserialize(
        "{\"name\":\"Γιώργος\",\"age\":30,\"has_house\":true}"
    );
    BTR_jsonDeserialize(
        "{\n  \"name\": \"आरव\",\n  \"age\": 26,\n  \"has_house\": false\n  }\n"
    );
    return 0;
}
