#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define GRID_WiDTH 10
#define GRID_HEIGHT 10
#define LINE_LEN 15

#else
#define INPUT_FILE "./input.txt"
#define GRID_WiDTH 100
#define GRID_HEIGHT 100
#define LINE_LEN 105
#endif

#define GRID_SAVE_SIZE 100

#define CYCLES 1000000000

typedef struct Grid_t
{
    char field[GRID_HEIGHT][GRID_WiDTH];
    int h;
    int w;
} Grid_t;

void Swap(char* c1, char* c2)
{
    char tmp = *c1;
    *c1      = *c2;
    *c2      = tmp;
}

int FindFreeRowUp(Grid_t grid, int col, int row)
{
    do
    {
        row--;
    } while (row >= 0 && grid.field[row][col] == '.');
    return row + 1;
}

int FindFreeRowDown(Grid_t grid, int col, int row)
{
    do
    {
        row++;
    } while (row < grid.h && grid.field[row][col] == '.');
    return row - 1;
}

int FindFreeColUp(Grid_t grid, int row, int col)
{
    do
    {
        col--;
    } while (col >= 0 && grid.field[row][col] == '.');
    return col + 1;
}

int FindFreeColDown(Grid_t grid, int row, int col)
{
    do
    {
        col++;
    } while (col < grid.w && grid.field[row][col] == '.');
    return col - 1;
}

void TilTLeverUp(Grid_t* grid)
{
    for (int i = 1; i < grid->h; i++) // row
    {
        for (int j = 0; j < grid->w; j++) // col
        {
            if (grid->field[i][j] == 'O')
            {
                int row = FindFreeRowUp(*grid, j, i);
                Swap(&grid->field[i][j], &grid->field[row][j]);
            }
        }
    }
}

void TilTLeverDown(Grid_t* grid)
{
    for (int i = grid->h - 1; i >= 0; i--) // row
    {
        for (int j = 0; j < grid->w; j++) // col
        {
            if (grid->field[i][j] == 'O')
            {
                int row = FindFreeRowDown(*grid, j, i);
                Swap(&grid->field[i][j], &grid->field[row][j]);
            }
        }
    }
}

void TilTLeverLeft(Grid_t* grid)
{

    for (int i = 1; i < grid->w; i++) // col
    {
        for (int j = 0; j < grid->h; j++) // row
        {
            if (grid->field[j][i] == 'O')
            {
                int row = FindFreeColUp(*grid, j, i);
                Swap(&grid->field[j][i], &grid->field[j][row]);
            }
        }
    }
}

void TilTLeverRight(Grid_t* grid)
{

    for (int i = grid->w - 1; i >= 0; i--) // col
    {
        for (int j = 0; j < grid->h; j++) // row
        {
            if (grid->field[j][i] == 'O')
            {
                int row = FindFreeColDown(*grid, j, i);
                Swap(&grid->field[j][i], &grid->field[j][row]);
            }
        }
    }
}

void PrintGrid(Grid_t grid)
{
    for (int i = 0; i < grid.h; i++)
    {
        printf("%d. ", i);

        for (int j = 0; j < grid.w; j++)
        {
            printf("%c", grid.field[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

bool GridEqual(Grid_t g1, Grid_t g2)
{
    for (int i = 0; i < g1.h; i++)
    {
        for (int j = 0; j < g1.w; j++)
        {
            if (g1.field[i][j] != g2.field[i][j])
                return false;
        }
    }
    return true;
}

int GridSeen(Grid_t* grids, Grid_t grid, int gridSavesIdx)
{
    for (int i = 0; i < gridSavesIdx; i++)
    {
        if (GridEqual(grids[i], grid))
        {
            return i;
        }
    }
    return -1;
}

int main()
{

    FILE* pInputFile;
    char line[LINE_LEN + 2];

    pInputFile = fopen(INPUT_FILE, "r");
    if (NULL == pInputFile)
    {
        printf("file cannot be opened\n");
        return -1;
    }

    Grid_t grid;
    grid.h = GRID_HEIGHT;
    grid.w = GRID_WiDTH;

    for (int i = 0; i < grid.h; i++)
    {
        fgets(line, sizeof(line), pInputFile);
        strncpy(grid.field[i], line, sizeof(char) * grid.w);
    }
    int gridSavesSize = GRID_SAVE_SIZE;
    int gridSavesIdx  = 0;
    Grid_t* gridSaves = malloc(sizeof(Grid_t) * gridSavesSize);
    memcpy(&gridSaves[gridSavesIdx], &grid, sizeof(grid));
    gridSavesIdx++;

    int gridSeenIdx = 0;
    bool saveGrid   = true;
    for (int i = 0; i < CYCLES; i++)
    {
        TilTLeverUp(&grid);    // NORTH
        TilTLeverLeft(&grid);  // WEST
        TilTLeverDown(&grid);  // SOUTH
        TilTLeverRight(&grid); // EAST

        if (saveGrid)
        {
            memcpy(&gridSaves[gridSavesIdx], &grid, sizeof(grid));
            gridSeenIdx = GridSeen(gridSaves, grid, gridSavesIdx);

            if (gridSeenIdx >= 0)
            {
                // printf("grid seen, SeenIdx: %d, GridIdx :%d\n", gridSeenIdx, gridSavesIdx);
                saveGrid = false;
                int loop = gridSavesIdx - gridSeenIdx;
                int add  = (int)((CYCLES - i) / loop) * loop - loop;
                i += add;
            }
            gridSavesIdx++;
            if (gridSavesIdx >= gridSavesSize)
            {
                gridSavesSize *= 2;
                gridSaves = realloc(gridSaves, sizeof(Grid_t) * gridSavesSize);
            }
        }
    }
    int sum = 0;

    for (int i = 0; i < grid.h; i++)
    {
        for (int j = 0; j < grid.w; j++)
        {
            if (grid.field[i][j] == 'O')
            {
                sum += (grid.h - i);
            }
        }
    }

    printf("sum: %d\n", sum);
    free(gridSaves);
    return 0;
}