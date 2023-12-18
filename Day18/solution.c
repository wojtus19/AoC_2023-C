#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define INIT_SIZE 14
#define GRID_HEIGHT 10
#define GRID_WIDTH 7
#define MIN_X 0
#define MIN_Y 0
#else
#define INPUT_FILE "./input.txt"
#define INIT_SIZE 784
#define GRID_HEIGHT 800
#define GRID_WIDTH 800
#define MIN_X 200
#define MIN_Y 300
#endif // EXAMPLE

#define LINE_LEN 20
#define COLOR_LEN 6
#define COLOR_LEN_STR "6"

typedef enum Direction_t
{
    NONE  = 0,
    UP    = 'U',
    DOWN  = 'D',
    RIGHT = 'R',
    LEFT  = 'L'
} Direction_t;

typedef struct Node_t
{
    Direction_t direction;
    int steps;
    char color[COLOR_LEN];
} Node_t;

typedef struct Vector_t
{
    Node_t* data;
    int size;
    int items;
} Vector_t;

typedef struct Field_t
{
    char tile;
    bool visited;
} Field_t;

Vector_t InitVector(int size)
{
    Vector_t v;
    v.size  = size;
    v.items = 0;
    v.data  = malloc(sizeof(Node_t) * size);
    if (v.data != NULL)
    {
        memset(v.data, 0, sizeof(Node_t) * size);
    }
    else
    {
        printf("Couldn't initialize vector.\n");
    }
    return v;
}

void AddToVector(Vector_t* vector, Node_t value)
{

    if (vector->items >= vector->size)
    {
        // printf("\nReallocating.\n");
        vector->data = realloc(vector->data, sizeof(Node_t) * vector->size * 2);
        if (vector->data == NULL)
        {
            printf("Failed memory allocation for vector\n");
            return;
        }
        memset(&vector->data[vector->size], 0, sizeof(Node_t) * vector->size);
        vector->size = vector->size * 2;
    }
    vector->data[vector->items] = value;
    vector->items++;
}

void PrintVector(Vector_t vector)
{
    for (int i = 0; i < vector.items; i++)
    {
        printf("%c %d (#%s)\n", vector.data[i].direction, vector.data[i].steps, vector.data[i].color);
    }
    printf("\n");
}

void FreeVector(Vector_t* vector)
{
    free(vector->data);
}

void PrintGrid(Field_t grid[GRID_HEIGHT][GRID_WIDTH])
{
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            printf("%c", grid[i][j].tile);
        }
        printf("\n");
    }
    printf("\n");
}

bool IsInBox(Field_t grid[GRID_HEIGHT][GRID_WIDTH], int y, int x)
{
    int crosses = 0;
    for (int i = x + 1; i < GRID_WIDTH; i++)
    {
        if (grid[y][i].visited  && grid[y + 1][i].visited && grid[y - 1][i].visited)
        {
            crosses++;
        }
        else if(grid[y][i].visited && (grid[y + 1][i].visited || grid[y - 1][i].visited))
        {
            bool upperCol = grid[y - 1][i].visited;
            while (grid[y][i].visited)
            {
                i++;
            }
            if((upperCol && grid[y + 1][i - 1].visited) || (!upperCol && grid[y - 1][i - 1].visited))
            {
                crosses++;
            }

        }
    }
    return crosses % 2;
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

    Vector_t digPlan = InitVector(INIT_SIZE);

    while (fgets(line, sizeof(line), pInputFile) != 0)
    {
        char dir;
        int steps;
        char color[COLOR_LEN];
        sscanf(line, "%c %d (#%6c)", &dir, &steps, color);

        Node_t node;
        node.direction = dir;
        node.steps     = steps;
        strncpy(node.color, color, sizeof(node.color));

        AddToVector(&digPlan, node);
    }

    // PrintVector(digPlan);

    Field_t grid[GRID_HEIGHT][GRID_WIDTH];
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            grid[i][j].tile    = '.';
            grid[i][j].visited = false;
        }
    }

    int dx   = 0;
    int dy   = 0;
    int posX = MIN_X;
    int posY = MIN_Y;
    for (int i = 0; i < digPlan.items; i++)
    {
        switch (digPlan.data[i].direction)
        {
        case UP:
            dx = 0;
            dy = -1;
            break;
        case DOWN:
            dx = 0;
            dy = 1;
            break;
        case RIGHT:
            dx = 1;
            dy = 0;
            break;
        case LEFT:
            dx = -1;
            dy = 0;
            break;
        default:
            dx = 0;
            dy = 0;
            break;
        }

        for (int x = 0; x < digPlan.data[i].steps; x++)
        {
            posX += 1 * dx;
            posY += 1 * dy;
            grid[posY][posX].tile    = '#';
            grid[posY][posX].visited = true;
        }
    }

    // PrintGrid(grid);

    for (int i = 5; i < GRID_HEIGHT - 5; i++)
    {
        for (int j = 5; j < GRID_WIDTH - 5; j++)
        {
            if (grid[i][j].tile == '.' && IsInBox(grid, i, j))
            {
                grid[i][j].visited = true;
            }
        }
    }

    int changed = 0;

    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            if (grid[i][j].visited || grid[i][j].tile == '#')
            {
                changed++;
                grid[i][j].tile = '#';
            }
        }
    }

    //PrintGrid(grid);
    printf("changed: %d\n", changed);

    FreeVector(&digPlan);
    return 0;
}