#include "Vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define GRID_WIDTH 10
#define GRID_HEIGHT 10
#define GALAXIES_NUMBER 9
#else
#define INPUT_FILE "./input.txt"
#define GRID_WIDTH 140
#define GRID_HEIGHT 140
#define GALAXIES_NUMBER 429
#endif

#define INIT_SIZE 16

typedef struct Coords_t
{
    int x;
    int y;
} Coords_t;

bool IsRowEmpty(Coords_t galaxies[], int whichRow, int numberOfGalaxies)
{
    for (int i = 0; i < numberOfGalaxies; i++)
    {
        if (galaxies[i].y == whichRow)
        {
            return false;
        }
    }
    return true;
}

bool IsColEmpty(Coords_t galaxies[], int whichCol, int numberOfGalaxies)
{
    for (int i = 0; i < numberOfGalaxies; i++)
    {
        if (galaxies[i].x == whichCol)
        {
            return false;
        }
    }
    return true;
}

int CountOccurences(int a, int b, int array[], int arraySize)
{
    int start  = a < b ? a : b;
    int finish = a > b ? a : b;

    int occurences = 0;
    for (int i = 0; i < arraySize; i++)
    {
        if (array[i] >= start && array[i] <= finish)
        {
            occurences++;
        }
    }
    return occurences;
}

void FillVectorWithIndexesOfEmptyLines(Vector_t* vector, int item_count, Coords_t* galaxies, int galaxiesNumber, bool (*comparator)(Coords_t*, int, int))
{
    for (int i = 0; i < item_count; i++)
    {
        if ((*comparator)(galaxies, i, galaxiesNumber))
        {
            AddToVector(vector, i);
        }
    }
}

int main()
{

    FILE* pInputFile;
    char line[GRID_HEIGHT + 2];

    pInputFile = fopen(INPUT_FILE, "r");
    if (NULL == pInputFile)
    {
        printf("file cannot be opened\n");
        return -1;
    }

    Coords_t galaxies[GALAXIES_NUMBER] = { 0 };
    int galaxyIdx                      = 0;

    char* p;
    for (int idx = 0; idx < GRID_HEIGHT; idx++)
    {
        p = fgets(line, sizeof(line), pInputFile);
        if (p == NULL)
            break;

        for (int j = 0; j < GRID_WIDTH; j++)
        {
            if (line[j] == '#')
            {
                galaxies[galaxyIdx].x = j;
                galaxies[galaxyIdx].y = idx;
                galaxyIdx++;
            }
        }
    }

    Vector_t emptyCols = InitVector(INIT_SIZE);
    Vector_t emptyRows = InitVector(INIT_SIZE);

    // FillVectorWithIndexesOfEmptyLines(&emptyCols, GRID_WIDTH, galaxies, GALAXIES_NUMBER, IsColEmpty);
    //
    // FillVectorWithIndexesOfEmptyLines(&emptyRows, GRID_HEIGHT, galaxies, GALAXIES_NUMBER, IsRowEmpty);

    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        if (IsRowEmpty(galaxies, i, GALAXIES_NUMBER))
        {
            AddToVector(&emptyRows, i);
        }
    }

    for (int i = 0; i < GRID_WIDTH; i++)
    {
        if (IsColEmpty(galaxies, i, GALAXIES_NUMBER))
        {
            AddToVector(&emptyCols, i);
        }
    }

    long long sum = 0;
    for (int i = 0; i < GALAXIES_NUMBER; i++)
    {
        for (int j = i + 1; j < GALAXIES_NUMBER; j++)
        {
            long long emptyColsBetweenGalaxies = (1000000 - 1) * CountOccurences(galaxies[i].x, galaxies[j].x, emptyCols.data, emptyCols.items);
            long long emptyRowsBetweenGalaxies = (1000000 - 1) * CountOccurences(galaxies[i].y, galaxies[j].y, emptyRows.data, emptyRows.items);

            long long distance = abs(galaxies[i].x - galaxies[j].x) + abs(galaxies[i].y - galaxies[j].y) + emptyColsBetweenGalaxies + emptyRowsBetweenGalaxies;
            sum += distance;
        }
    }

    printf("sum: %lld\n", sum);

    FreeVector(emptyCols);
    FreeVector(emptyRows);
    return 0;
}