#include "token.h"

Token newToken(TokenType type, char* literal, int intValue)
{
    Token ret;
    ret.type = type;
    ret.intValue = intValue;
    if(literal != NULL)
    {
        ret.literal = malloc(strlen(literal) + 1);
        strcpy(ret.literal, literal);
    }else
    {
        ret.literal = NULL;
    }
    return ret;
}

int isOperator(char c)
{
    if(c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>')
        return 1;
    return 0; 
}