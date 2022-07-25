#ifndef UTILS_H
#define UTILS_H

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
#include "defs.h"
#include "structs.h"

void printer(const u_int8_t (*sudoku)[SIZE]);
void initSudoku(u_int8_t (*sudoku)[SIZE]);

#endif