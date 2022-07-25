#include "../include/utils.h"

const u_int8_t* extractFromFile(const u_int8_t* filepath);
void charArrToByteMat(const u_int8_t* buffer, u_int8_t (*sudoku)[SIZE]);
void updateEntropyMat();
void initPossibleMat();
void updatePossibleMat(const u_int8_t val, const u_int8_t row, const u_int8_t col);
struct cell findBestCell();

u_int8_t g_entropy_mat[SIZE][SIZE];
u_int16_t g_possible_mat[SIZE][SIZE][SIZE];
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
    u_int8_t sudoku[SIZE][SIZE];
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
        if(best_cell.row == -1 && best_cell.col == -1)
            break;
        printf("best cell found at row %ld and column %ld with the value: %d\n", best_cell.row,
                                                                            best_cell.col,
                                                                            g_entropy_mat[best_cell.row][best_cell.col]);        
        //roll a dice to se what value to use
        srand(time(NULL));
        u_int32_t val = rand()%9 + 1;
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

const u_int8_t* extractFromFile(const u_int8_t* filepath)
{
    printf("Openning: %s\n", filepath);
    int32_t fd = open(filepath, O_RDONLY);

    if(fd < 0)
    {
        printf("File \"%s\" could not open\n", filepath);
        exit(2);
    }

    struct stat statbuf;
    int32_t status = fstat(fd, &statbuf);

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

void charArrToByteMat(const u_int8_t* buffer, u_int8_t (*sudoku)[SIZE])
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

void initPossibleMat()
{
    memset(g_possible_mat, 1, (sizeof g_possible_mat));
}

void updatePossibleMat(const u_int8_t val, const u_int8_t row, const u_int8_t col)
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
struct cell findBestCell()
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
