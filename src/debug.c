#include <stdio.h>
#include "debug.h"

void printArray(LineArray* array)
{
    int i = 0;
    for(;i < array->count;i++)
    {
        printf("== Line %d == \n",i);
        printLine(&array->lines[i]);
    } 
}

void printLine(Line* line)
{
    int i = 0;
    for(;i<line->count;i++)
    {
        printf("== Token %d == \n",i);
        if(line->tokens[i].literal != NULL)
            printf("%s\n",line->tokens[i].literal);
        else
            printf("%d\n",line->tokens[i].intValue);
    }
}