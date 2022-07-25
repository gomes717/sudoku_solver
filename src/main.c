#include "../include/utils.h"

const char* extractFromFile(const char* filepath);
void charArrToByteMat(const char* buffer, u_int8_t (*sudoku)[SIZE]);
void updateEntropyMat();
void initPossibleMat();
void updatePossibleMat(const u_int8_t val, const u_int8_t row, const u_int8_t col);
struct cell findBestCell();

size_t finish_count = SIZE*SIZE;
u_int8_t g_entropy_mat[SIZE][SIZE];
u_int16_t g_possible_mat[SIZE][SIZE][SIZE];

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("Missing an argument. Usage example: %s filepath/file.txt\n", argv[0]);
        exit(1);
    }
    
    u_int8_t sudoku[SIZE][SIZE];
    initSudoku(argv[1], sudoku);
    printf("Sudoku matrix:\n");
    printer(sudoku);

    // initPossibleMat();

    // updateEntropyMat();
    // printf("Entropy matrix initialize\n");
    // printer(g_entropy_mat);

    // printf("solving the sudoku\n");
    // while(finish_count > 0)
    // {
    //     //find the cell with less entropy
    //     struct cell best_cell = findBestCell();
    //     if(best_cell.row == -1 && best_cell.col == -1)
    //         break;
    //     printf("best cell found at row %d and column %d with the value: %d\n", best_cell.row,
    //                                                                         best_cell.col,
    //                                                                         g_entropy_mat[best_cell.row][best_cell.col]);        
    //     //roll a dice to se what value to use
    //     srand(time(NULL));
    //     u_int32_t val = rand()%9 + 1;
    //     //if the value is not possible at that cell find another
    //     while(g_possible_mat[best_cell.row][best_cell.col][val-1] == 0)
    //     {
    //         val = rand()%9 + 1;
    //     }
    //     sudoku[best_cell.row][best_cell.col] = val;
    //     updatePossibleMat(val, best_cell.row, best_cell.col);
    //     updateEntropyMat();
    //     finish_count--;
    //     printer(g_entropy_mat);
    //     printer(sudoku);
    //     printf("Value of finish_count: %ld\n", finish_count);
    // }

    // printf("Sudoku resolved\n");
    // printer(sudoku);
    return 0;
}
