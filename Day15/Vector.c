#include "Vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vector_t InitVector(int size)
{
    Vector_t v;
    v.size  = size;
    v.items = 0;
    v.data  = malloc(sizeof(lens_t) * size);
    if (v.data != NULL)
    {
        memset(v.data, 0, sizeof(lens_t) * size);
    }
    else
    {
        printf("Couldn't initialize vector.\n");
    }
    return v;
}

int LensExists(Vector_t vector, char* label)
{
    for (int i = 0; i < vector.items; i++)
    {
        if (strcmp(vector.data[i].label, label) == 0)
        {
            return i;
        }
    }
    return -1;
}

void AddToVector(Vector_t* vector, char* label, int value)
{

    int existIdx = LensExists(*vector, label);
    if (existIdx >= 0)
    {
        vector->data[existIdx].value = value;
        return;
    }

    if (vector->items >= vector->size)
    {
        vector->data = realloc(vector->data, sizeof(lens_t) * vector->size * 2);
        if (vector->data == NULL)
        {
            printf("Failed memory allocation for vector\n");
            return;
        }
        memset(&vector->data[vector->size], 0, sizeof(lens_t) * vector->size);
        vector->size = vector->size * 2;
    }
    if (strlen(label) < MAX_STR)
    {
        lens_t newLens;
        memset(newLens.label, 0, sizeof(char) * MAX_STR);
        strncpy(newLens.label, label, strlen(label));
        newLens.value               = value;
        vector->data[vector->items] = newLens;
        vector->items++;
    }
}

void DeleteFromVector(Vector_t* vector, char* label)
{
    int lensIdx = LensExists(*vector, label);
    if (lensIdx >= 0 && (lensIdx + 1 < vector->items))
    {
        memcpy(&vector->data[lensIdx], &vector->data[lensIdx + 1], sizeof(lens_t) * (vector->items - lensIdx));
        vector->items--;
    }
    else if (lensIdx >= 0)
    {
        vector->items--;
    }
}

void PrintVector(Vector_t vector)
{
    if (vector.items == 0)
    {
        return;
    }
    for (int i = 0; i < vector.items; i++)
    {
        printf("[%s %d] ", vector.data[i].label, vector.data[i].value);
    }
    printf("\n");
}

void FreeVector(Vector_t* vector)
{
    free(vector->data);
}