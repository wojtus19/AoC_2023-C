#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define GRID_SIZE 23
#define LINE_LEN 25
#else
#define INPUT_FILE "./input.txt"
#define GRID_SIZE 141
#define LINE_LEN 143
#endif // EXAMPLE

typedef struct Coords_t
{
    int x;
    int y;
} Coords_t;

bool LegalMove(char grid[GRID_SIZE][GRID_SIZE], int x, int y, int dx, int dy)
{
    if (x + dx >= GRID_SIZE || x + dx < 0 || y + dy >= GRID_SIZE || y + dy < 0)
        return false;
    if (grid[y + dy][x + dx] == '#' || (dx == -1 && grid[y + dy][x + dx] == '>') || (dy == -1 && grid[y + dy][x + dx] == 'v'))
        return false;
    return true;
}

int CalculateSteps(char grid[GRID_SIZE][GRID_SIZE], int x, int y, int dx, int dy, int steps)
{
    int directions[4][2] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };
    int newSteps         = steps;

    while (y != GRID_SIZE - 1 || x != GRID_SIZE - 2)
    {
        bool foundAtLeastOneWay = false;

        int oldDx = dx;
        int oldDy = dy;
        for (int i = 0; i < 4; i++)
        {
            if ((oldDy != 0 && directions[i][1] == (-1 * oldDy)) || (oldDx != 0 && directions[i][0] == (-1 * oldDx))) // dont go back
                continue;
            if (LegalMove(grid, x, y, directions[i][0], directions[i][1]))
            {
                if (!foundAtLeastOneWay)
                {
                    dx = directions[i][0];
                    dy = directions[i][1];

                    foundAtLeastOneWay = true;
                }
                else
                {
                    int tmpSteps = CalculateSteps(grid, x + directions[i][0], y + directions[i][1], directions[i][0], directions[i][1], steps + 1);
                    if (tmpSteps > newSteps)
                    {
                        newSteps = tmpSteps;
                    }
                }
            }
        }
        if (LegalMove(grid, x, y, dx, dy))
        {
            x += dx;
            y += dy;
            steps++;
        }
    }
    return steps > newSteps ? steps : newSteps;
}
int main()
{

    FILE* pInputFile;
    char line[LINE_LEN];

    pInputFile = fopen(INPUT_FILE, "r");
    if (NULL == pInputFile)
    {
        printf("file cannot be opened\n");
        return -1;
    }

    char grid[GRID_SIZE][GRID_SIZE];
    for (int i = 0; i < GRID_SIZE; i++)
    {
        fgets(line, sizeof(line), pInputFile);
        if (line == NULL)
            return -1;

        for (int j = 0; j < GRID_SIZE; j++)
        {
            grid[i][j] = line[j];
        }
    }
    int result = CalculateSteps(grid, 1, 0, 0, 1, 0);
    printf("result: %d\n", result);

    return 0;
}
