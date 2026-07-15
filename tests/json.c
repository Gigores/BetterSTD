#include "btrstd/json.h"
#include "assert.h"

int main(void)
{
    btr_json_value_s v1 = BTR_jsonDeserialize("10");
    assert(v1.type == BTR_JSON_NUMBER);
    assert(v1.number == 10);
    BTR_JsonValue_free(&v1);

    btr_json_value_s v2 = BTR_jsonDeserialize("\"Сергей\"");
    assert(v2.type == BTR_JSON_STRING);
    btr_string_view_s expected = BTR_StringView_fromCString("Сергей");
    assert(!BTR_StringView_compare(v2.string, expected));
    BTR_JsonValue_free(&v2);

    btr_json_value_s v3 = BTR_jsonDeserialize("true");
    assert(v3.type == BTR_JSON_BOOL);
    assert(v3.boolean);
    BTR_JsonValue_free(&v3);

    btr_json_value_s v4 = BTR_jsonDeserialize("false");
    assert(v4.type == BTR_JSON_BOOL);
    assert(!v4.boolean);
    BTR_JsonValue_free(&v4);

    btr_json_value_s v5 = BTR_jsonDeserialize("null");
    assert(v5.type == BTR_JSON_NULL);
    BTR_JsonValue_free(&v5);

    btr_json_value_s v6 = BTR_jsonDeserialize("[1, 2, 3, 4, 5]");
    assert(v6.type == BTR_JSON_ARRAY);
    assert(((btr_json_value_s *)BTR_unwrap(BTR_BLList_get(&v6.array, 0)))->number == 1);
    assert(((btr_json_value_s *)BTR_unwrap(BTR_BLList_get(&v6.array, 1)))->number == 2);
    assert(((btr_json_value_s *)BTR_unwrap(BTR_BLList_get(&v6.array, 2)))->number == 3);
    assert(((btr_json_value_s *)BTR_unwrap(BTR_BLList_get(&v6.array, 3)))->number == 4);
    assert(((btr_json_value_s *)BTR_unwrap(BTR_BLList_get(&v6.array, 4)))->number == 5);
    BTR_JsonValue_free(&v6);

    btr_json_value_s v7 = BTR_jsonDeserialize("[\"1\", \"2\", \"3\", \"4\", \"5\",]");
    assert(v7.type == BTR_JSON_ARRAY);
    {
        btr_string_view_s expected = BTR_StringView_fromCString("1");
        assert(!BTR_StringView_compare(((btr_json_value_s *)BTR_unwrap(BTR_BLList_get(&v7.array, 0)))->string, expected));
    } {
        btr_string_view_s expected = BTR_StringView_fromCString("2");
        assert(!BTR_StringView_compare(((btr_json_value_s *)BTR_unwrap(BTR_BLList_get(&v7.array, 1)))->string, expected));
    } {
        btr_string_view_s expected = BTR_StringView_fromCString("3");
        assert(!BTR_StringView_compare(((btr_json_value_s *)BTR_unwrap(BTR_BLList_get(&v7.array, 2)))->string, expected));
    } {
        btr_string_view_s expected = BTR_StringView_fromCString("4");
        assert(!BTR_StringView_compare(((btr_json_value_s *)BTR_unwrap(BTR_BLList_get(&v7.array, 3)))->string, expected));
    } {
        btr_string_view_s expected = BTR_StringView_fromCString("5");
        assert(!BTR_StringView_compare(((btr_json_value_s *)BTR_unwrap(BTR_BLList_get(&v7.array, 4)))->string, expected));
    }
    BTR_JsonValue_free(&v7);

    btr_json_value_s v8 = BTR_jsonDeserialize(
        "{\n  \"name\": \"John\",\n  \"age\": 34,\n  \"relatives\": [\n    \"Marie\",\n    \"Alex\"\n  ],\n  \"car\": null\n}"
    );
    assert(v8.type == BTR_JSON_OBJECT);
    {
        btr_json_value_s *name = BTR_unwrap(BTR_BHTable_get(&v8.object, "name"));
        assert(name->type == BTR_JSON_STRING);
        btr_string_view_s expected_name = BTR_StringView_fromCString("John");
        assert(!BTR_StringView_compare(name->string, expected_name));
    } {
        btr_json_value_s *age = BTR_unwrap(BTR_BHTable_get(&v8.object, "age"));
        assert(age->type == BTR_JSON_NUMBER);
        assert(age->number == 34);
    } {
        btr_json_value_s *car = BTR_unwrap(BTR_BHTable_get(&v8.object, "car"));
        assert(car->type == BTR_JSON_NULL);
    } {
        btr_json_value_s *relatives = BTR_unwrap(BTR_BHTable_get(&v8.object, "relatives"));
        assert(relatives->type == BTR_JSON_ARRAY);
        {
            btr_json_value_s *m = BTR_unwrap(BTR_BLList_get(&relatives->array, 0));
            assert(m->type == BTR_JSON_STRING);
            btr_string_view_s expected_m = BTR_StringView_fromCString("Marie");
            assert(!BTR_StringView_compare(m->string, expected_m));
        } {
            btr_json_value_s *a = BTR_unwrap(BTR_BLList_get(&relatives->array, 1));
            assert(a->type == BTR_JSON_STRING);
            btr_string_view_s expected_a = BTR_StringView_fromCString("Alex");
            assert(!BTR_StringView_compare(a->string, expected_a));
        }
    }
    BTR_JsonValue_free(&v8);
    return 0;
}
