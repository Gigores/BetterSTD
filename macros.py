import metapy as meta


def _parse_list(tokens: list[meta.Token]) -> list[list[meta.Token]]:
    result = []
    current = []
    paren_depth = 0
    bracket_depth = 0
    brace_depth = 0
    for token in tokens:
        match token.type:
            case meta.TokenType.LPAREN:
                paren_depth += 1
            case meta.TokenType.RPAREN:
                paren_depth -= 1
            case meta.TokenType.LBRACKET:
                bracket_depth += 1
            case meta.TokenType.RBRACKET:
                bracket_depth -= 1
            case meta.TokenType.LBRACE:
                brace_depth += 1
            case meta.TokenType.RBRACE:
                brace_depth -= 1
        if (
            token.type == meta.TokenType.COMMA
            and paren_depth == 0
            and bracket_depth == 0
            and brace_depth == 0
        ):
            result.append(current)
            current = []
            continue
        current.append(token)
    if current:
        result.append(current)
    return result


def balist(input: list[meta.Token]) -> list[meta.Token]:
    result: list[meta.Token] = []
    parsed: list[list[meta.Token]] = _parse_list(input)
    result.extend(meta.quote(f"({{btr_balist_s result = BTR_BAList_make({len(parsed)}, NULL);"))
    for item in parsed:
        sizeof: list[meta.Token] = meta.quote("sizeof(", *item, ")")
        result.extend(meta.quote(
            "BTR_BAList_append(&result, ({"
                "void *item = BTR_unwrap(BTR_allocate(", *sizeof, "));"
                # TODO
                "memcpy(", *sizeof, ", item, );"
            "}));"))
    result.extend(meta.quote("result;})"))


def init_macros():
    meta.register_token_macro("balist", balist, meta.MacroPairedDelims.SQUARE_BRACKET.value)

