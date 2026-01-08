#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"

#define MAX_LENGTH 128
#define MAX_LITERAL_LENGTH 128

#define BOUNDARY(c) ((c)==' ' || (c)=='\t' || (c)=='\0' || (c)=='\n')

typedef struct {
    int count;
    int capacity;
    Token* tokens;
} Line;

typedef struct {
    int count;
    int capacity;
    Line* lines;
} LineArray;

LineArray scanFile(FILE* file);
Line parseLine(char* line);

Line initLine();
LineArray initLineArray();

void addToken(Line* l, Token token);
void addLine(LineArray* array, Line line);

void freeLine(Line* line);
void freeLineArray(LineArray* array);

int parseIdentifier(char* line, int i, Line* l);
int parseIntLiteral(char* line, int i, Line* l);
int parseOperator(char* line, int i, Line* l);
int parseString(char* line, int i, Line* l);

#endif