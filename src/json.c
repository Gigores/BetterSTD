#ifdef BTR_INCLUDE_ALL

#include "btrstd/json.h"

#include "btrstd/string_view.h"
#include "btrstd/allocators/global.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    btr_string_view_s text;
    size_t curCharIndex;
    char *curChar;
    btr_bllist_s tokens;
    btr_allocator_s *allocator;
} token_state_t;

#define TOKEN_X_LIST \
    X_TOKEN_TYPE(LBRACE,   "{")    \
    X_TOKEN_TYPE(RBRACE,   "}")    \
    X_TOKEN_TYPE(COMMA,    ",")    \
    X_TOKEN_TYPE(COLON,    ":")    \
    X_TOKEN_TYPE(LBRACKET, "[")    \
    X_TOKEN_TYPE(RBRACKET, "]")    \
    X_TOKEN_TYPE(STRING,   "string") \
    X_TOKEN_TYPE(NUMBER,   "number") \
    X_TOKEN_TYPE(TRUE,     "true")   \
    X_TOKEN_TYPE(FALSE,    "false")  \
    X_TOKEN_TYPE(NULL,     "null")

typedef enum {
    #define X_TOKEN_TYPE(name, str) TOKEN_##name,
    TOKEN_X_LIST
    #undef X_TOKEN_TYPE
} token_type_t;

static const char *TokenType_toString(token_type_t type) {
    switch (type) {
        #define X_TOKEN_TYPE(name, str) case TOKEN_##name: return #name;
        TOKEN_X_LIST
        #undef X_TOKEN_TYPE
        default: return "unknown";
    }
}

typedef struct {
    token_type_t type;
    btr_string_view_s content;
} token_t;

static char *tokenizer_next(token_state_t *state)
{
    state->curCharIndex++;
    state->curChar = (char *) BTR_StringView_charAt(&state->text, state->curCharIndex);
    return state->curChar;
}
static bool tokenizer_canAdvance(token_state_t *state)
{
    return BTR_StringView_charAt(&state->text, state->curCharIndex) != NULL;
}

static void tokenizer_pushToken(token_state_t *state, token_type_t type, btr_string_view_s view)
{
    token_t *token = BTR_expect(
        BTR_Allocator_allocate(state->allocator, sizeof(token_t)),
        "Allocation failed"
    );
    token->type = type;
    token->content = view;
    BTR_BLList_append(&state->tokens, token);
}

static btr_string_view_s tokenizer_getCurCharView(token_state_t *state)
{
    return BTR_StringView_substring(&state->text, state->curCharIndex, 1);
}
static void tokenizer_handleString(token_state_t *state)
{
    size_t start = state->curCharIndex;
    size_t len = 0;
    tokenizer_next(state);
    while (tokenizer_canAdvance(state) && *state->curChar != '"')
    {
        len++;
        tokenizer_next(state);
    }
    tokenizer_pushToken(
        state,
        TOKEN_STRING,
        BTR_StringView_substring(&state->text, start + 1, len)
    );
    tokenizer_next(state);
}
static void tokenizer_handleNumber(token_state_t *state)
{
    size_t start = state->curCharIndex;
    size_t len = 0;
    while (tokenizer_canAdvance(state) && isdigit(*state->curChar))
    {
        len++;
        tokenizer_next(state);
    }
    tokenizer_pushToken(
        state,
        TOKEN_NUMBER,
        BTR_StringView_substring(&state->text, start, len)
    );
}
static void tokenizer_handleKeyword(token_state_t *state)
{
    btr_string_view_s trueSv = BTR_StringView_fromCString("true");
    btr_string_view_s falseSv = BTR_StringView_fromCString("false");
    btr_string_view_s nullSv = BTR_StringView_fromCString("null");
    size_t start = state->curCharIndex;
    size_t len = 0;
    while (tokenizer_canAdvance(state) && isalpha(*state->curChar))
    {
        len++;
        tokenizer_next(state);
    }
    btr_string_view_s sv = BTR_StringView_substring(&state->text, start, len);
    if (!BTR_StringView_compare(&sv, &trueSv))
        tokenizer_pushToken(
            state,
            TOKEN_TRUE,
            sv
        );
    else if (!BTR_StringView_compare(&sv, &falseSv))
        tokenizer_pushToken(
            state,
            TOKEN_FALSE,
            sv
        );
    else if (!BTR_StringView_compare(&sv, &nullSv))
        tokenizer_pushToken(
            state,
            TOKEN_NULL,
            sv
        );
}

static btr_bllist_s tokenize(const char *string, btr_allocator_s *allocator)
{
    token_state_t state = {
        .text = BTR_StringView_fromCString(string),
        .curChar = (char *)string,
        .tokens = BTR_BLList_make(NULL),
        .allocator = allocator,
    };
    while (tokenizer_canAdvance(&state))
        switch (*state.curChar)
        {
        case '{':
            tokenizer_pushToken(&state, TOKEN_LBRACE, tokenizer_getCurCharView(&state));
            tokenizer_next(&state);
            break;
        case '}':
            tokenizer_pushToken(&state, TOKEN_RBRACE, tokenizer_getCurCharView(&state));
            tokenizer_next(&state);
            break;
        case '[':
            tokenizer_pushToken(&state, TOKEN_LBRACKET, tokenizer_getCurCharView(&state));
            tokenizer_next(&state);
            break;
        case ']':
            tokenizer_pushToken(&state, TOKEN_RBRACKET, tokenizer_getCurCharView(&state));
            tokenizer_next(&state);
            break;
        case ':':
            tokenizer_pushToken(&state, TOKEN_COLON, tokenizer_getCurCharView(&state));
            tokenizer_next(&state);
            break;
        case ',':
            tokenizer_pushToken(&state, TOKEN_COMMA, tokenizer_getCurCharView(&state));
            tokenizer_next(&state);
            break;
        case '"':
            tokenizer_handleString(&state);
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            tokenizer_handleNumber(&state);
            break;
        default:
            if (isalpha(*state.curChar))
                tokenizer_handleKeyword(&state);
            else
                tokenizer_next(&state);
            break;
        }
    return state.tokens;
}
typedef struct {
    btr_bllist_s *tokens;
    size_t curTokenIndex;
    btr_allocator_s *allocator;
} parse_state_t;

static bool parser_canAdvance(parse_state_t *state)
{
    return state->curTokenIndex < BTR_BLList_len(state->tokens);
}
static token_t *parser_advance(parse_state_t *state)
{
    state->curTokenIndex++;
    return (token_t *)BTR_unwrap(BTR_BLList_get(state->tokens, state->curTokenIndex));
}
static token_t *parser_getNext(parse_state_t *state)
{
    return (token_t *)BTR_unwrap(BTR_BLList_get(state->tokens, state->curTokenIndex + 1));
}
static btr_json_value_s parse(parse_state_t *state)
{
    token_t *firstToken = (token_t *)BTR_unwrap(
        BTR_BLList_get(state->tokens, state->curTokenIndex)
    );
    if (firstToken->type == TOKEN_NUMBER)
        return (btr_json_value_s) {
            .type = BTR_JSON_NUMBER,
            .number = BTR_StringView_parseDouble(&firstToken->content),
            .allocator = state->allocator,
        };
    if (firstToken->type == TOKEN_STRING)
        return (btr_json_value_s) {
            .type = BTR_JSON_STRING,
            .string = firstToken->content,
            .allocator = state->allocator,
        };
    if (firstToken->type == TOKEN_TRUE)
        return (btr_json_value_s) {
            .type = BTR_JSON_BOOL,
            .boolean = true,
            .allocator = state->allocator,
        };
    if (firstToken->type == TOKEN_FALSE)
        return (btr_json_value_s) {
            .type = BTR_JSON_BOOL,
            .boolean = false,
            .allocator = state->allocator,
        };
    if (firstToken->type == TOKEN_NULL)
        return (btr_json_value_s) {
            .type = BTR_JSON_NULL,
            .allocator = state->allocator,
        };
    if (firstToken->type == TOKEN_LBRACKET)
    {
        token_t *next = parser_advance(state);
        btr_bllist_s values = BTR_BLList_make(NULL);
        while (next->type != TOKEN_RBRACKET)
        {
            btr_json_value_s *value = BTR_expect(
                BTR_Allocator_allocate(state->allocator, sizeof(btr_json_value_s)),
                "Allocation failed"
            );
            *value = parse(state);
            BTR_BLList_append(&values, value);
            next = parser_advance(state);
            if (next->type == TOKEN_RBRACKET)
                break;
            if (next->type != TOKEN_COMMA)
                BTR_panic("Invalid syntax");
            next = parser_advance(state);
        }
        return (btr_json_value_s) {
            .type = BTR_JSON_ARRAY,
            .array = values,
            .allocator = state->allocator,
        };
    }
    if (firstToken->type == TOKEN_LBRACE)
    {
        token_t *next = parser_advance(state);
        btr_bhtable_s values =
            BTR_BHTable_make(BTR_hashCString, BTR_compareCString, state->allocator);
        while (next->type != TOKEN_RBRACE)
        {
            if (next->type != TOKEN_STRING)
                BTR_panic("Invalid key. Expected String, got %s", TokenType_toString(next->type));
            char *key = BTR_expect(
                BTR_Allocator_allocate(state->allocator, next->content.length + 1),
                "Allocation failed",
            );
            memcpy(key, next->content.data, next->content.length);
            key[next->content.length] = '\0';

            next = parser_advance(state);
            if (next->type != TOKEN_COLON)
                BTR_panic("Invalid syntax");

            next = parser_advance(state);
            btr_json_value_s *value = BTR_expect(
                BTR_Allocator_allocate(state->allocator, sizeof(btr_json_value_s)),
                "Allocation failed"
            );
            *value = parse(state);
            BTR_BHTable_put(&values, key, value);
            next = parser_advance(state);
            if (next->type == TOKEN_RBRACE)
                break;
            if (next->type != TOKEN_COMMA)
                BTR_panic("Invalid syntax");
            next = parser_advance(state);
        }
        return (btr_json_value_s) {
            .type = BTR_JSON_OBJECT,
            .object = values,
            .allocator = state->allocator,
        };
    }
    BTR_panic("Invalid token: %s", TokenType_toString(firstToken->type));
}

btr_json_value_s BTR_jsonDeserialize(const char *string)
{
    btr_allocator_s *theAllocator = (btr_allocator_s *)BTR_getGlobalAllocator();
    btr_bllist_s tokens = tokenize(string, theAllocator);
    parse_state_t state = {
        .tokens = &tokens,
        .allocator = theAllocator,
    };
    // BTR_BLLIST_FOREACH(&tokens, i)
    //     printf("%s: |"BTR_STRING_FORMAT"|\n",
    //         TokenType_toString(((token_t *)i)->type),
    //         BTR_STRING_ARGS(((token_t *)i)->content));
    btr_json_value_s parsed = parse(&state);
    BTR_BLLIST_FOREACH(&tokens, i2)
        BTR_Allocator_deallocate(theAllocator, i2);
    BTR_BLList_free(&tokens);
    return parsed;
}

void BTR_JsonValue_free(btr_json_value_s *value)
{
    if (value->type == BTR_JSON_ARRAY)
    {
        BTR_BLLIST_FOREACH(&value->array, i)
        {
            BTR_JsonValue_free(i);
            BTR_Allocator_deallocate(value->allocator, i);
        }
        BTR_BLList_free(&value->array);
    }
    else if (value->type == BTR_JSON_OBJECT)
    {
        btr_bllist_s keys = BTR_BHTable_keys(&value->object);
        BTR_BLLIST_FOREACH(&keys, key)
        {
            btr_json_value_s *theValue = BTR_unwrap(BTR_BHTable_get(&value->object, key));
            BTR_JsonValue_free(theValue);
            BTR_Allocator_deallocate(value->allocator, theValue);
            BTR_Allocator_deallocate(value->allocator, key);
        }
        BTR_BLList_free(&keys);
        BTR_BHTable_free(&value->object);
    }
}

#endif  // BTR_INCLUDE_ALL
