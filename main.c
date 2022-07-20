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


int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("Missing an argument. Usage example: %s filepath/file.txt\n", argv[0]);
        exit(1);
    }

    const char* filepath = argv[1];
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
        return 1;
    }
    close(fd);

    printf("FILE INFO: \n%s\n", buff);

    char sudoku[SIZE][SIZE];
    size_t col = 0;
    size_t row = 0;
    size_t i = 0;
    while(row < SIZE)
    {
        if(buff[i] == '\n')
        {
            row++;
            col = 0;
            i++;
            continue;
        }

        if(buff[i] != ' ')
        {
            sudoku[row][col] = (char)buff[i] - '0';
            col++;
        }
        i++;
    }

    printer(sudoku);
    return 0;
}

