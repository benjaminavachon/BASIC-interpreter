#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "stack.h"
#include "token.h"

void initTokenStack(TokenStack* stack)
{
    stack->count = 0;
    stack->capacity = 8;
    stack->tokens = malloc(sizeof(Token)*8);
    if(!stack->tokens)
    {
        printf("error initializing TokenStack\n");
        exit(-1);
    }
}

Token pushToken(TokenStack* stack, Token token){
    if(stack->capacity < stack->count+1)
    {
        //resize array;
        stack->capacity *= 2;
        Token* temp = realloc(stack->tokens,sizeof(Token) * (stack->capacity));
        if(temp == NULL)
        {
            printf("SIZE GROW FAILED\n");
            exit(-1);
        }
        stack->tokens = temp;
    }

    stack->tokens[stack->count++] = token;
    return token;

}
Token peekToken(TokenStack* stack){
    return stack->tokens[stack->count-1];
}
Token popToken(TokenStack* stack){
    Token ret = stack->tokens[stack->count-1];

    stack->count--;

    return ret;
}

void initExecStack(ExecStack* stack)
{
    stack->count = 0;
    stack->capacity = 8;
    stack->lines = malloc(sizeof(int)*8);
    if(!stack->lines)
    {
        printf("error initializing ExecStack\n");
        exit(-1);
    }
}

int pushLine(ExecStack* stack, int line){
    if(stack->capacity < stack->count+1)
    {
        //resize array;
        stack->capacity *= 2;
        int* temp = realloc(stack->lines,sizeof(int) * (stack->capacity));
        if(temp == NULL)
        {
            printf("SIZE GROW FAILED\n");
            exit(-1);
        }
        stack->lines = temp;
    }

    stack->lines[stack->count++] = line;
    return line;
}
int peekLine(ExecStack* stack){
    return stack->lines[stack->count-1];
}
int popLine(ExecStack* stack){
    int ret = stack->lines[stack->count-1];
    stack->count--;

    return ret;
}