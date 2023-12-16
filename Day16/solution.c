#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define GRID_SIZE 10
#else
#define INPUT_FILE "./input.txt"
#define GRID_SIZE 110
#endif // EXAMPLE

#define LINE_LEN 120

typedef enum Direction_t
{
    NONE  = 0,
    UP    = 1,
    RIGHT = 2,
    DOWN  = 3,
    LEFT  = 4
} Direction_t;

typedef struct Coords_t
{
    int x;
    int y;
} Coords_t;

typedef struct Field_t
{
    char tile;
    bool visitedBy[5];
} Field_t;
Field_t grid[GRID_SIZE][GRID_SIZE];

bool IsBoundaries(Coords_t pos)
{
    return pos.x >= 0 && pos.x < GRID_SIZE && pos.y >= 0 && pos.y < GRID_SIZE;
}

void GetDelt(int* dx, int* dy, Direction_t dir)
{
    switch (dir)
    {
    case UP:
        *dy = -1;
        *dx = 0;
        break;
    case DOWN:
        *dy = 1;
        *dx = 0;
        break;
    case LEFT:
        *dx = -1;
        *dy = 0;
        break;
    case RIGHT:
        *dx = 1;
        *dy = 0;
        break;

    default:
        break;
    }
}

Direction_t SwitchSlash(Direction_t d)
{
    switch (d)
    {
    case UP:
        d = RIGHT;
        break;
    case DOWN:
        d = LEFT;
        break;
    case RIGHT:
        d = UP;
        break;
    case LEFT:
        d = DOWN;
        break;
    default:
        break;
    }
    return d;
}

Direction_t SwitchBackslash(Direction_t d)
{
    switch (d)
    {
    case UP:
        d = LEFT;
        break;
    case DOWN:
        d = RIGHT;
        break;
    case RIGHT:
        d = DOWN;
        break;
    case LEFT:
        d = UP;
        break;

    default:
        break;
    }
    return d;
}

bool Visited(Field_t f)
{
    for (int i = 0; i < 5; i++)
    {
        if (f.visitedBy[i] == true)
            return true;
    }
    return false;
}

void StartBeam(Coords_t pos, Direction_t direction)
{
    int dx = 0;
    int dy = 0;

    while (IsBoundaries(pos))
    {

        if (grid[pos.y][pos.x].visitedBy[direction] == true)
        {
            return;
        }
        else
        {
            grid[pos.y][pos.x].visitedBy[direction] = true;
        }

        switch (grid[pos.y][pos.x].tile)
        {
        case '.':
            // Do nothing
            break;
        case '|':
            if (direction == LEFT || direction == RIGHT)
            {
                StartBeam(pos, DOWN);
                StartBeam(pos, UP);
                return;
            }
            break;
        case '-':
            if (direction == UP || direction == DOWN)
            {
                StartBeam(pos, LEFT);
                StartBeam(pos, RIGHT);
                return;
            }
            break;
        case '/':
            direction = SwitchSlash(direction);
            break;
        case '\\':
            direction = SwitchBackslash(direction);
            break;

        default:
            break;
        }

        GetDelt(&dx, &dy, direction);
        pos.x += dx;
        pos.y += dy;
    }
}

int CalculateMax(int max)
{
    int sum = 0;
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (Visited(grid[i][j]))
            {
                sum++;
            }
        }
    }
    return sum > max ? sum : max;
}

void ResetGrid()
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            memset(grid[i][j].visitedBy, 0, sizeof(grid[i][j].visitedBy));
        }
    }
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

    char* p;
    for (int i = 0; i < GRID_SIZE; i++)
    {
        p = fgets(line, sizeof(line), pInputFile);
        if (p == NULL)
            return -1;

        for (int j = 0; j < GRID_SIZE; j++)
        {
            grid[i][j].tile = line[j];
            memset(grid[i][j].visitedBy, 0, sizeof(grid[i][j].visitedBy));
        }
    }

    int max = 0;
    Coords_t start;
    for (int i = 0; i < GRID_SIZE; i++)
    {
        ResetGrid();
        start.x = 0;
        start.y = i;
        StartBeam(start, RIGHT);
        max = CalculateMax(max);

        ResetGrid();
        start.x = GRID_SIZE - 1;
        start.y = i;
        StartBeam(start, LEFT);
        max = CalculateMax(max);

        ResetGrid();
        start.x = i;
        start.y = GRID_SIZE - 1;
        StartBeam(start, UP);
        max = CalculateMax(max);

        ResetGrid();
        start.x = i;
        start.y = 0;
        StartBeam(start, DOWN);
        max = CalculateMax(max);
    }

    printf("sum: %d\n", max);
    return 0;
}