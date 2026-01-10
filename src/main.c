#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "lexer.h"
#include "debug.h"
#include "interpreter.h"

int main(int argv, char* argc[])
{
    if(argv == 1)
    {
        printf("no file to parse...\n");
        return -1;
    }

    if(argv > 2)
    {
        printf("too many inputs...\n");
        return -1;
    }

    FILE *file = fopen(argc[1],"r");
    if(file == NULL)
    {
        printf("could not open file\n");
        return -1;
    }

    //create array of lines/tokens
    LineArray array = scanFile(file);
      
    //execute tokens
    interpretArray(&array);
    //printArray(&array);

    return 0;
}