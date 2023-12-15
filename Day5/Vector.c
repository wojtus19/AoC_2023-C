#include "Vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vector_t InitVector(int size)
{
    Vector_t v;
    v.size  = size;
    v.items = 0;
    v.data  = malloc(sizeof(llu) * size);
    if (v.data != NULL)
    {
        memset(v.data, 0, sizeof(llu) * size);
    }
    else
    {
        printf("Couldn't initialize vector.\n");
    }
    return v;
}

void AddToVector(Vector_t* vector, llu value)
{

    if (vector->items >= vector->size)
    {
        // printf("\nReallocating.\n");
        vector->data = realloc(vector->data, sizeof(llu) * vector->size * 2);
        if (vector->data == NULL)
        {
            printf("Failed memory allocation for vector\n");
            return;
        }
        memset(&vector->data[vector->size], 0, sizeof(llu) * vector->size);
        vector->size = vector->size * 2;
    }
    vector->data[vector->items] = value;
    vector->items++;
}

Vector_t CopyVector(Vector_t vector)
{
    Vector_t copy = InitVector(vector.size);
    for (int i = 0; i < vector.items; i++)
    {
        AddToVector(&copy, vector.data[i]);
    }
    return copy;
}

void PrintVector(Vector_t vector)
{
    for (int i = 0; i < vector.items; i++)
    {
        printf("%llu ", vector.data[i]);
    }
    printf("\n");
}

void PrintVectorChar(Vector_t vector)
{
    for (int i = 0; i < vector.items; i++)
    {
        printf("%c ", (char)vector.data[i]);
    }
    printf("\n");
}

void FreeVector(Vector_t* vector)
{
    free(vector->data);
}