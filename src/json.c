#include "btrstd/json.h"

#include "btrstd/string_view.h"
#include "btrstd/allocators/global.h"
#include <ctype.h>
#include <stdio.h>

typedef struct {
    btr_string_view_t text;
    size_t curCharIndex;
    char *curChar;
    btr_bllist_t tokens;
    btr_allocator_t *allocator;
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
    btr_string_view_t content;
} token_t;

static char *tokenizer_next(token_state_t *state)
{
    state->curCharIndex++;
    state->curChar = BTR_StringView_charAt(&state->text, state->curCharIndex);
    return state->curChar;
}
static bool tokenizer_canAdvance(token_state_t *state)
{
    return BTR_StringView_charAt(&state->text, state->curCharIndex) != NULL;
}

static void tokenizer_pushToken(token_state_t *state, token_type_t type, btr_string_view_t view)
{
    token_t *token = BTR_expect(
        BTR_Allocator_allocate(state->allocator, sizeof(token_t)),
        "Allocation failed"
    );
    token->type = type;
    token->content = view;
    BTR_BLList_append(&state->tokens, token);
}

static btr_string_view_t tokenizer_getCurCharView(token_state_t *state)
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
    btr_string_view_t true_sv = BTR_StringView_fromCString("true");
    btr_string_view_t false_sv = BTR_StringView_fromCString("false");
    btr_string_view_t null_sv = BTR_StringView_fromCString("null");
    size_t start = state->curCharIndex;
    size_t len = 0;
    while (tokenizer_canAdvance(state) && isalpha(*state->curChar))
    {
        len++;
        tokenizer_next(state);
    }
    btr_string_view_t sv = BTR_StringView_substring(&state->text, start, len);
    if (!BTR_StringView_compare(&sv, &true_sv))
        tokenizer_pushToken(
            state,
            TOKEN_TRUE,
            sv
        );
    else if (!BTR_StringView_compare(&sv, &false_sv))
        tokenizer_pushToken(
            state,
            TOKEN_FALSE,
            sv
        );
    else if (!BTR_StringView_compare(&sv, &null_sv))
        tokenizer_pushToken(
            state,
            TOKEN_NULL,
            sv
        );
}

static btr_bllist_t tokenize(const char *string, btr_allocator_t *allocator)
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

btr_json_value_t BTR_jsonDeserialize(const char *string)
{
    btr_allocator_t *theAllocator = (btr_allocator_t *)BTR_getGlobalAllocator();
    btr_bllist_t tokens = tokenize(string, theAllocator);
    BTR_BLLIST_FOREACH(&tokens, i)
    {
        fprintf(stderr, "%s: "BTR_STRING_FORMAT"\n",
            TokenType_toString(((token_t *)i)->type),
            BTR_STRING_ARGS(((token_t *)i)->content));
        BTR_Allocator_deallocate(theAllocator, i);
    }
    BTR_BLList_free(&tokens);
    return (btr_json_value_t) {0};
}

void BTR_JsonValue_free(btr_json_value_t *);
