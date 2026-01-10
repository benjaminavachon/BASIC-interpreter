#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdlib.h>

#include "token.h"
#include "lexer.h"

void interpretArray(LineArray* array);
void interpretLine(Line* line, int* pc, int* vars, int* gosubStack, int* sp, LineArray* array);
int parseTerm(Line* line, int* i, int* vars);

void handlePrint(Line* line, int* vars);
void handleInput(Line* line, int* vars);
void handleLet(Line* line, int* vars);

int parseFactor(Line* line, int* i, int* vars);
void handleIf(Line* line, int* pc,int* vars,LineArray* array);
void handleGoto(Line* line, int* pc,LineArray* array);
void handleGosub(Line* line, int* pc, int* gosubStack, int* sp, LineArray* array);
void handleRETURN(int* pc, int* gosubStack, int* sp);
void handleRem(Line* line, int* vars);

int findLineIndex(LineArray* array, int targetLine);

int evalExpression(Line* line, int* i, int* vars);
int parseExpression(Line* line, int* i, int* vars);
int parseTerm(Line* line, int* i, int* vars);
int parseFactor(Line* line, int* i, int* vars);

#endif