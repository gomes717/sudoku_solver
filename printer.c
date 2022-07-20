#include "utils.h"
#include <stdio.h>

void printer(const char (*sudoku)[SIZE])
{
    for(int i = 0; i < 25; i++)
    {
        printf("-");
    }
    printf("\n");

    for(int i = 0; i < SIZE; i++)
    {
        printf("| ");
        for(int j = 0; j < SIZE; j++)
        {
            printf("%d ", sudoku[i][j]);
            if((j+1)%3 == 0)
                printf("| ");
        }
        printf("\n");
        
        if((i+1)%3 == 0)
        {
            for(int i = 0; i < 25; i++)
            {
                printf("-");
            }
            printf("\n");
        }
    }
}