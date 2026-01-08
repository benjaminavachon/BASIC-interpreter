#ifndef STACK_H
#define STACK_H

#include "token.h"

typedef struct {
    int count;
    int capacity;
    int* lines;
} ExecStack;

typedef struct {
    int count;
    int capacity;
    Token* tokens;
} TokenStack;

Token pushToken(TokenStack* stack, Token token);
Token peekToken(TokenStack* stack);
Token popToken(TokenStack* stack);

int pushLine(ExecStack* stack, int line);
int peekLine(ExecStack* stack);
int popLine(ExecStack* stack);

#endif