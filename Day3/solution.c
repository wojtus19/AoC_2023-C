#include <stdbool.h>
#include <stdio.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define GRID_WIDTH 10
#define GRID_LENGTH 10
#else
#define INPUT_FILE "./input.txt"
#define GRID_WIDTH 140
#define GRID_LENGTH 140
#endif

#define LINE_LEN GRID_WIDTH + 2

bool isBoundaries(int x, int y)
{
    return (x >= 0 && x < GRID_LENGTH && y >= 0 && y < GRID_WIDTH);
}

int findNumber(char grid[GRID_LENGTH][LINE_LEN], int y, int x)
{
    while (isBoundaries(x, y) && (grid[y][x] >= '0' && grid[y][x] <= '9'))
    {
        x--;
    }
    x++;

    int foundNumber = 0;

    while (isBoundaries(x, y) && (grid[y][x] >= '0' && grid[y][x] <= '9'))
    {
        foundNumber = (foundNumber * 10) + (grid[y][x] - '0');
        x++;
    }

    return foundNumber;
}

int main()
{

    FILE* pInputFile;

    pInputFile = fopen(INPUT_FILE, "r");
    if (NULL == pInputFile)
    {
        printf("file cannot be opened\n");
        return -1;
    }

    char grid[GRID_LENGTH][LINE_LEN];

    for (int idx = 0; idx < GRID_LENGTH; idx++)
    {
        if (fgets(grid[idx], sizeof(grid[idx]), pInputFile) == 0)
        {
            return -1;
        }
    }

    int sum       = 0;
    int firstNum  = 0;
    int secondNum = 0;
    for (int i = 0; i < GRID_LENGTH; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            if (grid[i][j] == '*')
            {
                firstNum  = 0;
                secondNum = 0;

                for (int y = -1; y <= 1; y++)
                {
                    for (int x = -1; x <= 1; x++)
                    {
                        if (isBoundaries(i + y, j + x))
                        {
                            if (grid[i + y][j + x] >= '0' && grid[i + y][j + x] <= '9')
                            {
                                int tmp = findNumber(grid, i + y, j + x);
                                if (firstNum != tmp && firstNum != 0)
                                {
                                    secondNum = tmp;
                                }
                                else
                                {
                                    firstNum = tmp;
                                }
                            }
                        }
                    }
                }
                // printf("firstNum: %d, secondNum :%d, gear ratio: %d\n", firstNum, secondNum, firstNum * secondNum);
                sum += firstNum * secondNum;
            }
        }
    }

    printf("sum: %d\n", sum);

    return 0;
}