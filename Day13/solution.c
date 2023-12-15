#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (0)
#if EXAMPLE
#define INPUT_FILE "./example.txt"
#else
#define INPUT_FILE "./input.txt"
#endif
#define LINE_LEN 50
#define GRID_MAX 512

#define GRID_NUMBER 100

typedef struct GridSize_t
{
    int height;
    int width;
} GridSize_t;

void PrindGrid(char** grid, int height, int width)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

bool ColsEqual(char** grid, int c1, int c2, int height)
{
    for (int i = 0; i < height; i++)
    {
        if (grid[i][c1] != grid[i][c2])
        {
            return false;
        }
    }
    return true;
}

int CalculateCol(char** grid, int height, int width, int avoid)
{
    for (int i = 0; i < width - 1; i++)
    {
        if (ColsEqual(grid, i, i + 1, height))
        {
            int foundCol      = i + 1;
            int colOccurences = 0;
            while (foundCol - colOccurences - 1 >= 0 && foundCol + colOccurences < width)
            {
                if (ColsEqual(grid, foundCol - colOccurences - 1, foundCol + colOccurences, height))
                {
                    colOccurences++;
                }
                else
                {
                    goto continue_search_col;
                }
            }
            if (foundCol != avoid)
            {
                return foundCol;
            }
        }
    continue_search_col:
    }
    return 0;
}

bool RowsEqual(char** grid, int r1, int r2, int width)
{
    for (int i = 0; i < width; i++)
    {
        if (grid[r1][i] != grid[r2][i])
        {
            return false;
        }
    }
    return true;
}

int CalculateRow(char** grid, int height, int width, int avoid)
{
    for (int i = 0; i < height - 1; i++)
    {
        if (RowsEqual(grid, i, i + 1, width))
        {

            int foundRow      = i + 1;
            int rowOccurences = 1;
            while (foundRow - rowOccurences - 1 >= 0 && foundRow + rowOccurences < height)
            {
                if (RowsEqual(grid, foundRow - rowOccurences - 1, foundRow + rowOccurences, width))
                {
                    rowOccurences++;
                }
                else
                {
                    goto continue_search_row;
                }
            }
            if (foundRow != avoid)
            {
                return foundRow;
            }
        }
    continue_search_row:
    }
    return 0;
}

void Swap(char* c)
{
    if (*c == '#')
        *c = '.';
    else if (*c == '.')
        *c = '#';
}

long long int CalculateNumber(char** grid, int height, int width)
{

    int colNumber = 0;
    int rowNumber = 0;

    int oldCol = CalculateCol(grid, height, width, 0);
    int oldRow = CalculateRow(grid, height, width, 0);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            char** tmp = (char**)malloc(sizeof(char*) * height);
            for (int k = 0; k < height; k++)
            {
                tmp[k] = (char*)malloc(sizeof(char) * (width + 1));
                memset(tmp[k], 0, sizeof(char) * (width + 1));
                memcpy(tmp[k], grid[k], sizeof(char) * width);
            }

            Swap(&tmp[i][j]);

            int tmpFind = CalculateRow(tmp, height, width, oldRow);

            if (tmpFind > rowNumber && tmpFind != oldRow)
                rowNumber = tmpFind;

            tmpFind = CalculateCol(tmp, height, width, oldCol);
            if (tmpFind > colNumber && tmpFind != oldCol)
                colNumber = tmpFind;

            for (int k = 0; k < height; k++)
            {
                free(tmp[k]);
            }
            free(tmp);
        }
    }

    if (colNumber == 0 && rowNumber == 0)
    {
        colNumber = oldCol;
        rowNumber = oldRow;
    }

    if (colNumber > rowNumber)
    {
        return colNumber;
    }
    else
    {
        return 100 * rowNumber;
    }
}

int main()
{

    FILE* pInputFile;
    FILE* pInputFile2;
    char line[LINE_LEN + 2];
    char line2[LINE_LEN + 2];

    pInputFile = fopen(INPUT_FILE, "r");
    if (NULL == pInputFile)
    {
        printf("file cannot be opened\n");
        return -1;
    }
    pInputFile2 = fopen(INPUT_FILE, "r");
    if (NULL == pInputFile)
    {
        printf("file cannot be opened\n");
        return -1;
    }

    int gridHeight = 0;
    int gridWidth  = 0;

    long long int sum = 0;

    for (int gridIdx = 0; gridIdx < GRID_NUMBER; gridIdx++)
    {
        do
        {
            fgets(line, sizeof(line), pInputFile);
            if (strlen(line) <= 1)
                break;
            gridWidth = strlen(line) - 1;
            gridHeight++;
        } while (strlen(line) > 1);

        char** grid = (char**)malloc(sizeof(char*) * (gridHeight));
        for (int i = 0; i < gridHeight; i++)
        {
            grid[i] = (char*)malloc(sizeof(char) * (gridWidth + 1));
            memset(grid[i], 0, sizeof(char) * (gridWidth + 1));
        }

        int i = 0;
        do
        {
            fgets(line2, sizeof(line2), pInputFile2);

            if (strlen(line2) <= 1)
                break;

            strncpy(grid[i], line2, gridWidth);

            i++;
        } while (strlen(line2) > 1);

        long long int partial = CalculateNumber(grid, gridHeight, gridWidth);

        sum += partial;

        gridHeight = 0;
        gridHeight = 0;

        for (int i = 0; i < gridHeight; i++)
        {
            free(grid[i]);
        }
        free(grid);
    }

    printf("sum: %lld\n", sum);

    return 0;
}