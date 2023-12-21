#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define GRID_SIZE 11
#define LINE_LEN 15
#define EMPTY_COUNT 80
#else
#define INPUT_FILE "./input.txt"
#define GRID_SIZE 131
#define LINE_LEN 135
#define EMPTY_COUNT 14994
#endif // EXAMPLE

typedef struct Coords_t
{
    int x;
    int y;
} Coords_t;

void PrintGrid(char grid[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

bool InStart(Coords_t starts[EMPTY_COUNT], int startsCount, int x, int y)
{
    for (int i = 0; i < startsCount; i++)
    {
        if (starts[i].x == x && starts[i].y == y)
            return true;
    }
    return false;
}

int MakeSteps(char grid[GRID_SIZE][GRID_SIZE], Coords_t starts[EMPTY_COUNT], int* startsCount)
{
    int reachedPlots     = 0;
    int directions[4][2] = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };

    Coords_t newStarts[EMPTY_COUNT];
    int newStartsCount = 0;

    for (int i = 0; i < *startsCount; i++)
    {
        for (int dirIdx = 0; dirIdx < 4; dirIdx++)
        {
            int currX = starts[i].x + directions[dirIdx][1];
            int currY = starts[i].y + directions[dirIdx][0];
            if (currX >= 0 && currX < GRID_SIZE && currY >= 0 && currY < GRID_SIZE &&
                grid[currY][currX] != '#' && !InStart(newStarts, newStartsCount, currX, currY))
            {
                reachedPlots++;
                Coords_t currCord         = { .x = currX, .y = currY };
                newStarts[newStartsCount] = currCord;
                newStartsCount++;
            }
        }
    }
    memcpy(starts, newStarts, sizeof(newStarts));
    *(startsCount) = newStartsCount;

    return reachedPlots;
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
    Coords_t start = { .x = 0, .y = 0 };

    for (int i = 0; i < GRID_SIZE; i++)
    {
        fgets(line, sizeof(line), pInputFile);
        if (line == NULL)
            return -1;

        for (int j = 0; j < GRID_SIZE; j++)
        {
            grid[i][j] = line[j];
            if (line[j] == 'S')
            {
                start.x = j;
                start.y = i;
            }
        }
    }

    Coords_t starts[EMPTY_COUNT];
    starts[0]       = start;
    int startsCount = 1;

    int reachedPlots = 0;
    for (int i = 0; i < 64; i++)
    {
        reachedPlots = MakeSteps(grid, starts, &startsCount);
    }

    printf("steps: %d\n", reachedPlots);

    return 0;
}
