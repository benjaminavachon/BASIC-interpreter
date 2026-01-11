#include "interpreter.h"

void interpretArray(LineArray* array)
{
    int pc = 0;
    int vars[26] = {0};
    int gosubStack[32];
    int sp = 0;

    while(pc < array->count)
    {
        Line* line = &array->lines[pc];
        interpretLine(line, &pc, vars, gosubStack, &sp, array);
    }

}

void interpretLine(Line* line, int* pc, int* vars, int* gosubStack, int* sp, LineArray* array)
{
    Token t = line->tokens[1];
    switch(t.type)
    {
        case KW_PRINT: handlePrint(line, vars);break;
        case KW_INPUT: handleInput(line, vars);break;
        case KW_LET:   handleLet(line, vars);break;
        case KW_IF:    handleIf(line, pc, vars, array);return;
        case KW_GOTO:  handleGoto(line,pc, array);return;
        case KW_GOSUB: handleGosub(line,pc, gosubStack, sp, array);return;
        case KW_RETURN: handleRETURN(pc, gosubStack, sp);return;
        case KW_END:    exit(0);
        case KW_REM:    handleRem(line,vars);break;
    }
    (*pc)++;
}

void handleLet(Line* line, int* vars)
{
    //start at token after LET

    int i = 2;

    if(!isalpha(line->tokens[i].literal[0]))
    {
        printf("variables can only be single letters A-Z!\n");
        exit(1);
    }

    char variable = line->tokens[i].literal[0];
    i+=2;
    int value = evalExpression(line, &i, vars);

    vars[variable - 'A'] = value;
}

void handleIf(Line* line, int* pc, int* vars, LineArray* array)
{
    int i = 2;   // token index after IF

    // left expression
    int left = evalExpression(line, &i, vars);

    // relational operator
    Token op = line->tokens[i];
    i++;

    // right expression
    int right = evalExpression(line, &i, vars);

    // THEN keyword
    if (line->tokens[i].type != KW_THEN) {
        printf("Syntax error: expected THEN\n");
        exit(1);
    }
    i++;

    // target line number
    int targetLine = line->tokens[line->count-1].intValue;
    int idx = findLineIndex(array, targetLine);

    if (idx < 0) {
        printf("IF THEN jump to non-existent line %d\n", targetLine);
        exit(1);
    }

    // evaluate condition
    int condition = 0;
    switch (op.type) {
        case OP_LT: condition = (left < right); break;
        case OP_GT: condition = (left > right); break;
        case OP_EQ: condition = (left == right); break;
        case OP_NEQ: condition = (left != right); break;
        case OP_LTE: condition = (left <= right); break;
        case OP_GTE: condition = (left >= right); break;
    }

    if (condition) {
        *pc = idx;
        return;   // IMPORTANT
    }

    // false → fall through
}


void handleGoto(Line* line, int* pc,LineArray* array) 
{
    
    int targetLine = line->tokens[2].intValue;
    
    int idx = findLineIndex(array, targetLine);
    
    if(idx < 0)
    {
        printf("couldnt find line\n");
        exit(1);
    }
    
    *pc = idx;
}
void handleGosub(Line* line, int* pc, int* gosubStack, int* sp, LineArray* array) 
{
    gosubStack[*sp] = *pc + 1;
    (*sp)++;
    
    int targetLine = line->tokens[2].intValue;
    
    int idx = findLineIndex(array, targetLine);
    
    if(idx < 0)
    {
        printf("couldnt find line\n");
        exit(1);
    }
    
    *pc = idx;
}

int findLineIndex(LineArray* array, int targetLine)
{
    int i = 0;
    for(; i < array->count; i++)
    {
        if(array->lines[i].count != 0 && array->lines[i].tokens[0].intValue == targetLine)
        {
            return i;
        }
    }
    return -1;
}

void handleRETURN(int* pc, int* gosubStack, int* sp) 
{
    if (*sp <= 0) { printf("RETURN with empty GOSUB stack (sp=%d)\n", *sp); exit(1); }
    (*sp)--;
    *pc = gosubStack[*sp];
}
void handleRem(Line* line, int* vars) {}
void handleInput(Line* line, int* vars) 
{
    int i = 2;

    if(!isalpha(line->tokens[i].literal[0]))
    {
        printf("variables can only be single letters A-Z!\n");
        exit(1);
    }

    char variable = toupper(line->tokens[i].literal[0]);
    if (variable < 'A' || variable > 'Z') 
    { 
        printf("variables must be A-Z\n"); 
        exit(1);
    }

    int value;
    scanf("%i",&value);

    vars[variable - 'A'] = value;    
}

void handlePrint(Line* line, int* vars)
{
    int i = 2; //skip line number and PRINT
    int hasNewline = 1;
    while(i < line->count)
    {
        Token t = line->tokens[i];
        switch(t.type)
        {
            case TOK_STRING_LITERAL: 
                printf("%s",t.literal);
                i++;
                break;
            case SEMICOLON: 
                if(i == line->count-1)
                {
                    hasNewline = 0;i++;break;
                }
            case COMMA: 
                printf(" ");
                i++;
                break;
            default:
                int value = evalExpression(line, &i, vars);
                printf("%d", value);
                break;
        }
    }
    if(hasNewline)
    {
        printf("\n");
    }
}

int evalExpression(Line* line, int* i, int* vars)
{
    return parseExpression(line, i, vars);
}

int parseExpression(Line* line, int* i, int* vars)
{
    int value = parseTerm(line, i, vars);

    while (*i < line->count)
    {
        Token t = line->tokens[*i];

        if (t.type == OP_PLUS)
        {
            (*i)++;
            value += parseTerm(line, i, vars);
        }
        else if (t.type == OP_MINUS)
        {
            (*i)++;
            value -= parseTerm(line, i, vars);
        }
        else break;
    }

    return value;
}

int parseTerm(Line* line, int* i, int* vars)
{
    int value = parseFactor(line, i, vars);

    while (*i < line->count)
    {
        Token t = line->tokens[*i];

        if (t.type == OP_MUL)
        {
            (*i)++;
            value *= parseFactor(line, i, vars);
        }
        else if (t.type == OP_DIV)
        {
            (*i)++;
            value /= parseFactor(line, i, vars);
        }
        else break;
    }

    return value;
}

int parseFactor(Line* line, int* i, int* vars)
{
    Token t = line->tokens[*i];

    if (t.type == OP_MINUS) {
        (*i)++;
        return -parseFactor(line, i, vars);
    }


    if (t.type == TOK_INTEGER_LITERAL)
    {
        (*i)++;
        return t.intValue;
    }

    if (t.type == TOK_IDENTIFIER)
    {
        (*i)++;
        return vars[t.literal[0] - 'A'];
    }

    if (t.type == TOK_LPAREN)
    {
        (*i)++;
        int value = parseExpression(line, i, vars);
        (*i)++; // skip RPAREN
        return value;
    }

    printf("Syntax error in expression\n");
    exit(1);
}
