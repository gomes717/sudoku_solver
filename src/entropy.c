#include "../include/utils.h"

static void updateEntropyMat()
{
    for(size_t i = 0; i < SIZE; i++)
    {
        for(size_t j = 0; j < SIZE; j++)
        {
            u_int8_t entropy = 0;
            for(size_t k = 0; k < SIZE; k++)
            {
                entropy += g_possible_mat[i][j][k];
            }
            g_entropy_mat[i][j] = entropy;
        }
    }
}

static void initPossibleMat()
{
    memset(g_possible_mat, 1, (sizeof g_possible_mat));
    // if(sudoku[row][col] != 0){
    //             updatePossibleMat(sudoku[row][col], row, col);
    //             finish_count--;
    //         }
}

static void updatePossibleMat(const u_int8_t val, const u_int8_t row, const u_int8_t col)
{
    //update cell
    for(size_t i = 0; i < SIZE; i++)
    {
        g_possible_mat[row][col][i] = 0; 
    }
    //update column
    for(size_t i = 0; i < SIZE; i++)
    {
        g_possible_mat[i][col][val-1] = 0; 
    }
    //update row
    for(size_t i = 0; i < SIZE; i++)
    {
        g_possible_mat[row][i][val-1] = 0; 
    }
    //update block
    //find block
    const u_int8_t block = (col/3) + (row/3)*3;
    const u_int8_t row_block = (block/3)*3;
    const u_int8_t col_block = (block%3)*3;
    for(size_t i = row_block ; i < row_block + 3; i++)
    {
        for(size_t j = col_block; j < col_block + 3; j++)
        {
            g_possible_mat[i][j][val-1] = 0;
        }
    }
}