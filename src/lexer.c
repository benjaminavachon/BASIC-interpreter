
#include "lexer.h"

LineArray scanFile(FILE* file)
{
    LineArray array = initLineArray();
    char line[MAX_LENGTH];
    while(fgets(line,MAX_LENGTH,file))
    {
        Line l = parseLine(line);
        addLine(&array, l);
    }
    return array;
}

Line parseLine(char* line)
{
    Line l = initLine();

    int i = 0;
    int lineNumber = 0;

    //remove whitespace at beginning of line
    while(line[i] == ' ' || line[i] == '\t')
        i++;
    
    if(line[i] == '\0' || line[i] == '\n')
    {
        return l;
    }

    //check for line number.
    if(!isdigit((unsigned char)line[i]))
    {
        printf("LINE MUST START WITH NUMBER\n");
        exit(-1);
    }
    while(isdigit((unsigned char)line[i]))
    {
        lineNumber = 10*lineNumber + (line[i] - '0');
        i++;
    }

    Token t = newToken(TOK_INTEGER_LITERAL, NULL,lineNumber);
    addToken(&l,t);

    while(line[i] != '\0' && line[i] != '\n')
    {   
        //white space
        if(line[i] == ' ' || line[i] == '\t' || line[i] == '\r') {i++;continue;}
        
        //keywords
        if(strncmp(&line[i],"REM",3) == 0 && BOUNDARY(line[i+3])) 
        {
            return l;
        }
        if(strncmp(&line[i],"RETURN",6) == 0 && BOUNDARY(line[i+6])) 
        {
            addToken(&l,newToken(KW_RETURN,"RETURN",0));
            i+=6;
            continue;
        }
        if(strncmp(&line[i],"GOSUB",5) == 0 && BOUNDARY(line[i+5])) 
        {
            addToken(&l,newToken(KW_GOSUB,"GOSUB",0));
            i+=5;
            continue;
        }
        if(strncmp(&line[i],"PRINT",5) == 0 && BOUNDARY(line[i+5])) 
        {
            addToken(&l,newToken(KW_PRINT,"PRINT",0));
            i+=5;
            continue;
        }
        if(strncmp(&line[i],"INPUT",5) == 0 && BOUNDARY(line[i+5])) 
        {
            addToken(&l,newToken(KW_INPUT,"INPUT",0));
            i+=5;
            continue;
        }
        if(strncmp(&line[i],"THEN",4) == 0 && BOUNDARY(line[i+4])) 
        {
            addToken(&l,newToken(KW_THEN,"THEN",0));
            i+=4;
            continue;
        }
        if(strncmp(&line[i],"GOTO",4) == 0 && BOUNDARY(line[i+4])) 
        {
            addToken(&l,newToken(KW_GOTO,"GOTO",0));
            i+=4;
            continue;
        }
        if(strncmp(&line[i],"LET",3) == 0 && BOUNDARY(line[i+3])) 
        {
            addToken(&l,newToken(KW_LET,"LET",0));
            i+=3;
            continue;
        }
        if(strncmp(&line[i],"END",3) == 0 && BOUNDARY(line[i+3])) 
        {
            addToken(&l,newToken(KW_END,"END",0));
            i+=3;
            continue;
        }
        if(strncmp(&line[i],"IF",2) == 0 && BOUNDARY(line[i+2])) 
        {
            addToken(&l,newToken(KW_IF,"IF",0));
            i+=2;
            continue;
        }

        //identifiers
        if (isalpha(line[i])) 
        {
            i = parseIdentifier(line, i, &l);
            continue;
        }
        else if (isdigit(line[i])) 
        {
            i = parseIntLiteral(line, i, &l);
            continue;
        }
        else if (isOperator(line[i])) 
        {
            i = parseOperator(line, i, &l);
            continue;
        }
        else if (line[i] == '"') 
        {
            i = parseString(line, i, &l);
            continue;
        }
        else if(line[i] == ',')
        {
            addToken(&l,newToken(COMMA,",",0));
            i++;
            continue;
        }
        else if(line[i] == ';')
        {
            addToken(&l,newToken(SEMICOLON,";",0));
            i++;
            continue;
        }
        else if(line[i] == ':')
        {
            addToken(&l,newToken(COLON,":",0));
            i++;
            continue;
        }
        else if(line[i] == '(')
        {
            addToken(&l,newToken(TOK_LPAREN,"(",0));
            i++;
            continue;
        }

        else if(line[i] == ')')
        {
            addToken(&l,newToken(TOK_RPAREN,")",0));
            i++;
            continue;
        }
        else
        {
            addToken(&l,newToken(TOK_ERROR,"UNKOWN CHARACTER",0));
            i++;
            continue;
        }
    }

    return l;
}

Line initLine()
{
    Line l;
    l.capacity = 0;
    l.count = 0;
    l.tokens = NULL;
    return l;
}
LineArray initLineArray()
{
    LineArray array;
    array.capacity = 0;
    array.count = 0;
    array.lines = NULL;
    return array;
}

void addToken(Line* l, Token token)
{
    if(l->capacity < l->count + 1)
    {
        l->capacity = l->capacity == 0 ?  8 : l->capacity*2 ;
        Token* temp = realloc(l->tokens, sizeof(Token) * l->capacity);
        if(temp == NULL)
        {
            printf("error reallocating line tokens\n");
            exit(-1);
        }
        l->tokens = temp;
    }
    l->tokens[l->count++] = token;
}
void addLine(LineArray* array, Line line)
{
    if(array->capacity < array->count + 1)
    {
        array->capacity = array->capacity == 0 ?  8 : array->capacity*2 ;
        Line* temp = realloc(array->lines, sizeof(Line) * array->capacity);
        if(temp == NULL)
        {
            printf("error reallocating line tokens\n");
            exit(-1);
        }
        array->lines = temp;
    }
    array->lines[array->count++] = line;
}

void freeLine(Line* line)
{
    for (int i = 0; i < line->count; i++) {
        if (line->tokens[i].literal != NULL)
            free(line->tokens[i].literal);
    }

    free(line->tokens);
    line->count = 0;
    line->capacity = 0;
    line->tokens = NULL;
}
void freeLineArray(LineArray* array)
{
    int i = 0;
    for(;i < array->count;i++)
    {
        freeLine(&array->lines[i]);
    }

    free(array->lines);
    array->count = 0;
    array->capacity = 0;
    array->lines = NULL;
}

int parseIdentifier(char* line, int i, Line* l)
{
    char literal[MAX_LITERAL_LENGTH];
    int j = 0;
    while(isalnum((unsigned char)line[i]))
    {
        literal[j++] = line[i];
        i++;
    }
    literal[j] = '\0';
    addToken(l,newToken(TOK_IDENTIFIER,literal,0));
    return i;
}
int parseIntLiteral(char* line, int i, Line* l)
{
    int intLit = 0;
    while(isdigit((unsigned char)line[i]))
    {
        intLit = 10*intLit + (line[i] - '0');
        i++;
    }
    addToken(l,newToken(TOK_INTEGER_LITERAL,NULL,intLit));
    return i;
}
int parseOperator(char* line, int i, Line* l)
{
     if(line[i] == '<' && line[i+1] == '>')
    {
        addToken(l,newToken(OP_NEQ,"<>",0));
        return i+2;
    }
    if(line[i] == '<' && line[i+1] == '=')
    {
        addToken(l,newToken(OP_LTE,"<=",0));
        return i+2;
    }
    if(line[i] == '>' && line[i+1] == '=')
    {
        addToken(l,newToken(OP_GTE,">=",0));
        return i+2;
    }
    switch(line[i])
    {   
        case '=': addToken(l,newToken(OP_EQ,"=",0));break;
        case '+': addToken(l,newToken(OP_PLUS,"+",0));break;
        case '-': addToken(l,newToken(OP_MINUS,"-",0));break;
        case '*': addToken(l,newToken(OP_MUL,"*",0));break;
        case '/': addToken(l,newToken(OP_DIV,"/",0));break;
        case '<': addToken(l,newToken(OP_LT,"<",0));break;
        case '>': addToken(l,newToken(OP_GT,">",0));break;
    }

   
    return i+1;
}
int parseString(char* line, int i, Line* l)
{  
    char str[MAX_LITERAL_LENGTH];
    int j = 0;
    i++;
    while(line[i] != '"')
    {
        if(j >= MAX_LITERAL_LENGTH - 1)
        {
            addToken(l,newToken(TOK_ERROR,"STRING TO LONG",0));
            return i;
        }
        if(line[i] == '\n' || line[i] == '\0')
        {
            //no end quote
            addToken(l,newToken(TOK_ERROR,"NO END QUOTE",0));
            return i;
        }
        str[j++] = line[i];
        i++;
    }
    str[j] = '\0';
    addToken(l,newToken(TOK_STRING_LITERAL,str,0));
    i++;
    return i;
}