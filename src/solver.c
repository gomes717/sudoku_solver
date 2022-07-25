#include "../include/utils.h"

static struct cell findBestCell()
{   
    //bruteforce, find a better way to search for a best cell
    struct cell best_cell = { .col = -1, .row = -1};
    u_int8_t best_val = 10;
    for(size_t i = 0 ; i < SIZE; i++)
    {
        for(size_t j = 0; j < SIZE; j++)
        {
            if(best_val > g_entropy_mat[i][j] && g_entropy_mat[i][j] > 0)
            {
                best_cell.col = j;
                best_cell.row = i;
                best_val = g_entropy_mat[i][j];
            }
        }
    }
    return best_cell;
}
