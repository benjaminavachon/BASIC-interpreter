#ifndef TOKEN_H
#define TOKEN_H

#include <string.h>

typedef enum {
    TOK_EOF,
    TOK_ERROR,

    TOK_IDENTIFIER,
    TOK_INTEGER_LITERAL,
    TOK_STRING_LITERAL,

    KW_PRINT,
    KW_INPUT,
    KW_LET,
    KW_IF,
    KW_THEN,
    KW_GOTO,
    KW_GOSUB,
    KW_RETURN,
    KW_END,
    KW_REM,

    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV,
    OP_EQ,
    OP_LT,
    OP_GT,
    OP_NEQ,
    OP_LTE,
    OP_GTE,

    PAREN_OPEN,
    PAREN_CLOSE,
    COMMA,
    SEMICOLON,
    COLON,
    QUOTE,

    TOK_NEWLINE
} TokenType;

typedef struct {
    TokenType type;
    char* literal;
    int intValue;
} Token;

Token newToken(TokenType type, char* literal, int intValue);

int isOperator(char c);


#endif