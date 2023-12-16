#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define GRID_SIZE 5
#define START_DIR SOUTH
#else
#define INPUT_FILE "./input.txt"
#define GRID_SIZE 140
#define START_DIR SOUTH
#endif // EXAMPLE

#define LINE_LEN 200

typedef enum Direction_t
{
    NONE  = 0,
    NORTH = 1,
    EAST  = 2,
    SOUTH = 3,
    WEST  = 4
} Direction_t;

typedef struct Coords_t
{
    int x;
    int y;
} Coords_t;

typedef enum Pipe_t
{
    NORTH_TO_SOUTH = '|',
    EAST_TO_WEST   = '-',
    NORTH_TO_EAST  = 'L',
    NORTH_TO_WEST  = 'J',
    SOUTH_TO_WEST  = '7',
    SOUTH_TO_EAST  = 'F',
    GROUND         = '.',
    START          = 'S'
} Pipe_t;

typedef struct Field_t
{
    char tile;
    bool visited;
} Field_t;

void PrintGrid(Field_t grid[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (grid[i][j].visited)
                printf("#");
            else
                printf("O");
        }
        printf("\n");
    }
    printf("\n");
}

Coords_t FindStart(Field_t grid[GRID_SIZE][GRID_SIZE])
{
    Coords_t retval = { .x = 0, .y = 0 };
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (grid[i][j].tile == START)
            {
                retval.x = j;
                retval.y = i;
            }
        }
    }
    return retval;
}

void GetDelt(int* dx, int* dy, Direction_t dir)
{
    switch (dir)
    {
    case NORTH:
        *dy = -1;
        *dx = 0;
        break;
    case SOUTH:
        *dy = 1;
        *dx = 0;
        break;
    case WEST:
        *dx = -1;
        *dy = 0;
        break;
    case EAST:
        *dx = 1;
        *dy = 0;
        break;

    default:
        break;
    }
}

Direction_t GetDirection(Pipe_t pipe, Direction_t currentDir)
{
    switch (pipe)
    {
    case NORTH_TO_SOUTH:
        if (currentDir == NORTH)
            return NORTH;
        else if (currentDir == SOUTH)
            return SOUTH;

    case EAST_TO_WEST:
        if (currentDir == WEST)
            return WEST;
        else if (currentDir == EAST)
            return EAST;

    case NORTH_TO_EAST:
        if (currentDir == SOUTH)
            return EAST;
        else if (currentDir == WEST)
            return NORTH;

    case NORTH_TO_WEST:
        if (currentDir == SOUTH)
            return WEST;
        else if (currentDir == EAST)
            return NORTH;

    case SOUTH_TO_WEST:
        if (currentDir == EAST)
            return SOUTH;
        else if (currentDir == NORTH)
            return WEST;

    case SOUTH_TO_EAST:
        if (currentDir == NORTH)
            return EAST;
        else if (currentDir == WEST)
            return SOUTH;
    case GROUND:
    case START:
    default:
        return currentDir;
    }
}

int FindLoop(Field_t grid[GRID_SIZE][GRID_SIZE], Coords_t position)
{
    Pipe_t pipe     = grid[position.y][position.x].tile;
    Direction_t dir = START_DIR;
    int dx;
    int dy;
    int steps = 0;
    do
    {
        steps++;
        GetDelt(&dx, &dy, dir);
        position.x += dx;
        position.y += dy;

        grid[position.y][position.x].visited = true;

        pipe = grid[position.y][position.x].tile;
        dir  = GetDirection(pipe, dir);

    } while (pipe != START);
    return steps;
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

    Field_t grid[GRID_SIZE][GRID_SIZE];

    char* p;
    for (int i = 0; i < GRID_SIZE; i++)
    {
        p = fgets(line, sizeof(line), pInputFile);
        if (p == NULL)
            return -1;
        for (int j = 0; j < GRID_SIZE; j++)
        {
            grid[i][j].tile    = line[j];
            grid[i][j].visited = false;
        }
    }

    Coords_t start = FindStart(grid);

    int loopSize = FindLoop(grid, start);

    int result = loopSize / 2;
    // PrintGrid(grid);

    printf("loopsize: %d, result: %d\n", loopSize, result);
    return 0;
}