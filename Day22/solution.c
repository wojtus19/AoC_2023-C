#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define BRICKS_NUM 7
#else
#define INPUT_FILE "./input.txt"
#define BRICKS_NUM 1468
#endif // EXAMPLE

#define LINE_LEN 20

typedef struct Coords_t
{
    int x;
    int y;
    int z;
} Coords_t;

typedef struct Brick_t
{
    Coords_t start;
    Coords_t end;
} Brick_t;

void PrintBricks(Brick_t* bricks, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d, %d, %d ~ %d, %d, %d\n", bricks[i].start.x, bricks[i].start.y, bricks[i].start.z, bricks[i].end.x, bricks[i].end.y, bricks[i].end.z);
    }
    printf("\n");
}

bool BricksEqual(Brick_t b1, Brick_t b2)
{
    return (b1.start.x == b2.start.x && b1.start.y == b2.start.y && b1.start.z == b2.start.z && b1.end.x == b2.end.x && b1.end.y == b2.end.y && b1.end.z == b2.end.z);
}

bool BricksOverlap(Brick_t b1, Brick_t b2)
{
    int b1xSmaller = b1.start.x < b1.end.x ? b1.start.x : b1.end.x;
    int b1xBigger  = b1.start.x > b1.end.x ? b1.start.x : b1.end.x;

    int b2xSmaller = b2.start.x < b2.end.x ? b2.start.x : b2.end.x;
    int b2xBigger  = b2.start.x > b2.end.x ? b2.start.x : b2.end.x;

    int b1ySmaller = b1.start.y < b1.end.y ? b1.start.y : b1.end.y;
    int b1yBigger  = b1.start.y > b1.end.y ? b1.start.y : b1.end.y;

    int b2ySmaller = b2.start.y < b2.end.y ? b2.start.y : b2.end.y;
    int b2yBigger  = b2.start.y > b2.end.y ? b2.start.y : b2.end.y;

    if (((b1xSmaller >= b2xSmaller && b1xSmaller <= b2xBigger) || (b2xSmaller >= b1xSmaller && b2xSmaller <= b1xBigger)) && // x overlapp
        ((b1ySmaller >= b2ySmaller && b1ySmaller <= b2yBigger) || (b2ySmaller >= b1ySmaller && b2ySmaller <= b1yBigger)))   // and y overlap
        return true;
    return false;
}

int CountSupporters(Brick_t* bricks, int size, Brick_t brick)
{
    int supporters = 0;
    for (int i = 0; i < size; i++)
    {
        if (bricks[i].end.z + 1 == brick.start.z && BricksOverlap(bricks[i], brick) && !BricksEqual(bricks[i], brick))
            supporters++;
    }
    return supporters;
}

bool CanBeDisintegrated(Brick_t* bricks, int size, Brick_t brick)
{
    bool canBeDisintegrated = true;
    for (int i = size - 1; i >= 0; i--)
    {
        if (BricksEqual(bricks[i], brick))
            break;

        if ((bricks[i].start.z == brick.end.z + 1 && BricksOverlap(bricks[i], brick) && CountSupporters(bricks, size, bricks[i]) == 1))
        {
            canBeDisintegrated = false;
            break;
        }
    }

    return canBeDisintegrated;
}

int CmpBricks(const void* a, const void* b)
{
    Brick_t* b1 = (Brick_t*)a;
    Brick_t* b2 = (Brick_t*)b;

    return b1->start.z - b2->start.z;
}

bool IsMinimum(Brick_t* bricks, int size, Brick_t b)
{
    if (b.start.z == 1)
        return true;

    for (int i = 0; i < size; i++)
    {
        if (BricksEqual(bricks[i], b))
            return false;

        if (bricks[i].end.z + 1 == b.start.z && BricksOverlap(bricks[i], b))
        {
            return true;
        }
    }
    return false;
}

int FallBricks(Brick_t* bricks, int size)
{
    bool count = true;
    int fallen = 0;
    for (int i = 0; i < size; i++)
    {
        while (!IsMinimum(bricks, size, bricks[i]))
        {
            if (count)
            {
                count = false;
                fallen++;
            }
            bricks[i].start.z--;
            bricks[i].end.z--;
        }
        count = true;
    }
    return fallen;
}

void SwapCoords(Brick_t* brick)
{
    Coords_t temp = brick->start;
    brick->start  = brick->end;
    brick->end    = temp;
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

    Brick_t bricks[BRICKS_NUM];

    for (int i = 0; i < BRICKS_NUM; i++)
    {
        fgets(line, sizeof(line), pInputFile);

        sscanf(line, "%d,%d,%d~%d,%d,%d\n", &bricks[i].start.x, &bricks[i].start.y, &bricks[i].start.z, &bricks[i].end.x, &bricks[i].end.y, &bricks[i].end.z);
        if (bricks[i].start.z > bricks[i].end.z)
        {
            SwapCoords(&bricks[i]);
        }
    }

    qsort(bricks, BRICKS_NUM, sizeof(Brick_t), CmpBricks);

    FallBricks(bricks, BRICKS_NUM);

    int desintegratedSum = 0;
    for (int i = BRICKS_NUM - 1; i >= 0; i--)
    {
        if (CanBeDisintegrated(bricks, BRICKS_NUM, bricks[i]))
            desintegratedSum += 1;
    }

    printf("Can be desintegrated: %d\n", desintegratedSum);

    int chainReactionSum = 0;
    Brick_t cpyBricks[BRICKS_NUM];
    for (int i = 0; i < BRICKS_NUM; i++)
    {
        memcpy(cpyBricks, bricks, sizeof(cpyBricks));
        Brick_t tmp               = cpyBricks[i];
        cpyBricks[i]              = cpyBricks[BRICKS_NUM - 1];
        cpyBricks[BRICKS_NUM - 1] = tmp;
        qsort(cpyBricks, BRICKS_NUM - 1, sizeof(Brick_t), CmpBricks);

        chainReactionSum += FallBricks(cpyBricks, BRICKS_NUM - 1);
    }

    printf("Chain reaction: %d\n", chainReactionSum);

    return 0;
}
