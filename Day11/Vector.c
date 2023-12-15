#include "Vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vector_t InitVector(int size)
{
    Vector_t v;
    v.size  = size;
    v.items = 0;
    v.data  = (int*)malloc(sizeof(int) * size);
    if (v.data != NULL)
    {
        memset(v.data, 0, sizeof(int) * size);
    }
    else
    {
        printf("Couldn't initialize vector.\n");
    }
    return v;
}

void AddToVector(Vector_t* vector, int value)
{

    if (vector->items >= vector->size)
    {
        printf("\nReallocating.\n");
        vector->data = (int*)realloc(vector->data, sizeof(int) * vector->size * 2);
        if (vector->data == NULL)
        {
            printf("Failed memory allocation for vector\n");
            return;
        }
        memset(&vector->data[vector->size], 0, sizeof(int) * vector->size);
        vector->size = vector->size * 2;
    }
    vector->data[vector->items] = value;
    vector->items++;
}

void PrintVector(Vector_t vector)
{
    for (int i = 0; i < vector.items; i++)
    {
        printf("%d ", vector.data[i]);
    }
    printf("\n");
}

void FreeVector(Vector_t* vector)
{
    free(vector->data);
}