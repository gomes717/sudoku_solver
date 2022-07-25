#ifndef UTILS_H
#define UTILS_H

#define SIZE 9

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include "sudoku.h"
struct cell
{
    size_t row;
    size_t col;
};

void printer(const u_int8_t (*sudoku)[SIZE]);

#endif