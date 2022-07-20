#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utils.h"
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

const char* extractFromFile(const char* filepath);
void charArrToByteMat(const char* buffer, char (*sudoku)[SIZE]);
void updateEntropyMat();
void initPossibleMat();
void updatePossibleMat(int val, int row, int col);

char g_entropy_mat[SIZE][SIZE];
char g_possible_mat[SIZE][SIZE][SIZE];


int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("Missing an argument. Usage example: %s filepath/file.txt\n", argv[0]);
        exit(1);
    }
    
    const char *buff = extractFromFile(argv[1]);
    //printf("FILE INFO: \n%s\n", buff);

    initPossibleMat();
    char sudoku[SIZE][SIZE];
    charArrToByteMat(buff, sudoku);
    printf("Sudoku matrix:\n");
    printer(sudoku);

    updateEntropyMat();
    printf("Entropy matrix initialize\n");
    printer(g_entropy_mat);
    return 0;
}

const char* extractFromFile(const char* filepath)
{
    printf("Openning: %s\n", filepath);
    int fd = open(filepath, O_RDONLY);

    if(fd < 0)
    {
        printf("File \"%s\" could not open\n", filepath);
        exit(2);
    }

    struct stat statbuf;
    int status = fstat(fd, &statbuf);

    if(status < 0)
    {
        printf("file \"%s\" could not open\n", filepath);
        exit(2);
    }

    char *buff = mmap(NULL, statbuf.st_size, PROT_READ,MAP_PRIVATE, fd, 0);
    if(buff == MAP_FAILED){
        printf("Mapping Failed\n");
        exit(3);
    }
    close(fd);
    return buff;
}

void charArrToByteMat(const char* buffer, char (*sudoku)[SIZE])
{
    size_t col = 0;
    size_t row = 0;
    size_t i = 0;
    while(row < SIZE)
    {
        if(buffer[i] == '\n')
        {
            row++;
            col = 0;
            i++;
            continue;
        }

        if(buffer[i] != ' ')
        {
            sudoku[row][col] = (char)buffer[i] - '0';
            if(sudoku[row][col] != 0)
                updatePossibleMat(sudoku[row][col], row, col);
            col++;
        }
        i++;
    }
}

void updateEntropyMat()
{
    for(int i = 0; i < SIZE; i++)
    {
        for(int j = 0; j < SIZE; j++)
        {
            int entropy = 0;
            for(int k = 0; k < SIZE; k++)
            {
                entropy += g_possible_mat[i][j][k];
            }
            g_entropy_mat[i][j] = entropy;
        }
    }
}

void initPossibleMat()
{
    memset(g_possible_mat, 1, (sizeof g_possible_mat));
}

void updatePossibleMat(int val, int row, int col)
{
    //update cell
    for(int i = 0; i < SIZE; i++)
    {
        g_possible_mat[row][col][i] = 0; 
    }
    //update column
    for(int i = 0; i < SIZE; i++)
    {
        g_possible_mat[i][col][val-1] = 0; 
    }
    //update row
    for(int i = 0; i < SIZE; i++)
    {
        g_possible_mat[row][i][val-1] = 0; 
    }
    //update block
    //find block
    int block = (col/3) + (row/3)*3;
    int row_block = (block/3)*3;
    int col_block = (block%3)*3;
    for(int i = row_block ; i < row_block + 3; i++)
    {
        for(int j = col_block; j < col_block + 3; j++)
        {
            g_possible_mat[i][j][val-1] = 0;
        }
    }
}