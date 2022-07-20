#include "utils.h"

const char* extractFromFile(const char* filepath);
void charArrToByteMat(const char* buffer, char (*sudoku)[SIZE]);
void updateEntropyMat();
void initPossibleMat();
void updatePossibleMat(int val, int row, int col);
struct cell findBestCell();

char g_entropy_mat[SIZE][SIZE];
char g_possible_mat[SIZE][SIZE][SIZE];
size_t finish_count = SIZE*SIZE;


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

    printf("solving the sudoku\n");
    while(finish_count > 0)
    {
        //find the cell with less entropy
        struct cell best_cell = findBestCell();
        printf("best cell found at row %ld and column %ld with the value: %d\n", best_cell.row,
                                                                            best_cell.col,
                                                                            g_entropy_mat[best_cell.row][best_cell.col]);        
        //roll a dice to se what value to use
        srand(time(NULL));
        int val = rand()%9 + 1;
        //if the value is not possible at that cell find another
        while(g_possible_mat[best_cell.row][best_cell.col][val-1] == 0)
        {
            val = rand()%9 + 1;
        }
        sudoku[best_cell.row][best_cell.col] = val;
        updatePossibleMat(val, best_cell.row, best_cell.col);
        updateEntropyMat();
        finish_count--;
        printer(g_entropy_mat);
        printer(sudoku);
        printf("Value of finish_count: %ld\n", finish_count);
    }

    printf("Sudoku resolved\n");
    printer(sudoku);
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
            if(sudoku[row][col] != 0){
                updatePossibleMat(sudoku[row][col], row, col);
                finish_count--;
            }
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
struct cell findBestCell()
{   
    //bruteforce, find a better way to search for a best cell
    struct cell best_cell = { .col = 0, .row = 0};
    int best_val = 10;
    for(int i = 0 ; i < SIZE; i++)
    {
        for(int j = 0; j < SIZE; j++)
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
