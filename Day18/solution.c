#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define INIT_SIZE 14
#else
#define INPUT_FILE "./input.txt"
#define INIT_SIZE 784
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

typedef struct Coords_t
{
    int x;
    int y;
} Coords_t;

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

    Coords_t* vertexes = (Coords_t*)malloc(sizeof(Coords_t) * (digPlan.items + 1));

    int dx                  = 0;
    int dy                  = 0;
    Coords_t first          = { .x = 0, .y = 0 };
    vertexes[0]             = first;
    vertexes[digPlan.items] = first;

    int vertexIdx = 0;
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

        Coords_t newVertex;
        newVertex.x = vertexes[vertexIdx].x + (dx * digPlan.data[i].steps);
        newVertex.y = vertexes[vertexIdx].y + (dy * digPlan.data[i].steps);
        vertexIdx++;
        vertexes[vertexIdx] = newVertex;
    }

    //for (int i = 0; i <= digPlan.items; i++)
    //{
    //    printf("(%d, %d)\n", vertexes[i].x, vertexes[i].y);
    //}

    // Calculate area with Shoelace formula

    int area = 0;
    for (int i = 1; i < digPlan.items; i++)
    {
        area += (vertexes[i - 1].x - vertexes[i + 1].x) * vertexes[i].y;
    }
    area = abs(area) / 2;

    printf("area: %d\n", area);

    int boundaryPoints = 0;
    for (int i = 0; i < digPlan.items; i++)
    {
        boundaryPoints += digPlan.data[i].steps;
    }

    // Calculate interior points with Pick's theorem
    // i = interior points, b = boundary points, A = area
    // A = i + b/2 - 1
    // i = A - b/2 + 1

    int interiorPoints = area - (boundaryPoints / 2) + 1;
    printf("interiorPoints: %d\n", interiorPoints);

    // interior points + boundary points = all points in polygon
    int pointsSum = interiorPoints + boundaryPoints;

    printf("pointsSum: %d\n", pointsSum);

    FreeVector(&digPlan);
    return 0;
}