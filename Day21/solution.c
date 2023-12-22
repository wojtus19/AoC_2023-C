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

typedef long long int lld_t;
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

lld_t MakeSteps(char grid[GRID_SIZE][GRID_SIZE], Coords_t starts[EMPTY_COUNT], int* startsCount)
{
    lld_t reachedPlots   = 0;
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

lld_t CountSteps(char grid[GRID_SIZE][GRID_SIZE], int x, int y, int steps)
{
    Coords_t s = { .x = x, .y = y };
    Coords_t starts[EMPTY_COUNT];
    starts[0]          = s;
    int startsCount    = 1;
    lld_t reachedPlots = 0;
    for (int i = 0; i < steps; i++)
    {
        reachedPlots = MakeSteps(grid, starts, &startsCount);
    }
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

    int reachedPlots = CountSteps(grid, start.x, start.y, 64);

    printf("steps: %d\n", reachedPlots);

    lld_t steps = 26501365;

    lld_t newGridWidth = (steps / GRID_SIZE) - 1;

    lld_t oddGrids  = newGridWidth * newGridWidth;
    lld_t evenGrids = (newGridWidth + 1) * (newGridWidth + 1);

    lld_t oddPoints  = CountSteps(grid, start.x, start.y, GRID_SIZE * 2 + 1);
    lld_t evenPoints = CountSteps(grid, start.x, start.y, GRID_SIZE * 2);

    lld_t topCorner    = CountSteps(grid, GRID_SIZE - 1, start.y, GRID_SIZE - 1);
    lld_t rightCorner  = CountSteps(grid, start.x, 0, GRID_SIZE - 1);
    lld_t bottomCorner = CountSteps(grid, 0, start.y, GRID_SIZE - 1);
    lld_t leftCorner   = CountSteps(grid, start.x, GRID_SIZE - 1, GRID_SIZE - 1);

    lld_t smallTopRightChunk    = CountSteps(grid, GRID_SIZE - 1, 0, (GRID_SIZE / 2) - 1);
    lld_t smallTopLeftChunk     = CountSteps(grid, GRID_SIZE - 1, GRID_SIZE - 1, (GRID_SIZE / 2) - 1);
    lld_t smallBottomRightChunk = CountSteps(grid, 0, 0, (GRID_SIZE / 2) - 1);
    lld_t smallBottomLeftChunk  = CountSteps(grid, 0, GRID_SIZE - 1, (GRID_SIZE / 2) - 1);

    lld_t bigTopRightChunk    = CountSteps(grid, GRID_SIZE - 1, 0, ((GRID_SIZE * 3) / 2) - 1);
    lld_t bigTopLeftChunk     = CountSteps(grid, GRID_SIZE - 1, GRID_SIZE - 1, ((GRID_SIZE * 3) / 2) - 1);
    lld_t bigBottomRightChunk = CountSteps(grid, 0, 0, ((GRID_SIZE * 3) / 2) - 1);
    lld_t bigBottomLeftChunk  = CountSteps(grid, 0, GRID_SIZE - 1, ((GRID_SIZE * 3) / 2) - 1);

    lld_t result = (oddGrids * oddPoints) + (evenGrids * evenPoints) + topCorner + rightCorner + bottomCorner + leftCorner + ((newGridWidth + 1) * (smallBottomLeftChunk + smallBottomRightChunk + smallTopLeftChunk + smallTopRightChunk)) + ((newGridWidth * (bigBottomLeftChunk + bigBottomRightChunk + bigTopLeftChunk + bigTopRightChunk)));

    printf("result: %lld\n", result);

    return 0;
}
