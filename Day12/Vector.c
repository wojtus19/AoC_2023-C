#include "Vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vector_t InitVector(int size)
{
    Vector_t v;
    v.size  = size;
    v.items = 0;
    v.data  = malloc(sizeof(int) * size);
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
        // printf("\nReallocating.\n");
        vector->data = realloc(vector->data, sizeof(int) * vector->size * 2);
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

Vector_t CopyVector(Vector_t vector)
{
    Vector_t copy = InitVector(vector.size);
    // memcpy(&copy.data, vector.data, sizeof(int) * vector.items);
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
        printf("%d ", vector.data[i]);
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

void AddToArrangementsVector(ArrangamentsResultVector_t* vector, ArrangamentsResult_t value)
{

    if (vector->items >= vector->size)
    {
        vector->items = 0;
        // printf("\nReallocating.\n");
        // vector->data = realloc(vector->data, sizeof(ArrangamentsResult_t) * vector->size * 2);
        // if (vector->data == NULL)
        //{
        //     printf("Failed memory allocation for vector\n");
        //     return;
        // }
        // memset(&vector->data[vector->size], 0, sizeof(ArrangamentsResult_t) * vector->size);
        // vector->size = vector->size * 2;
    }
    vector->data[vector->items] = value;
    vector->items++;
}

ArrangamentsResultVector_t InitArrangementVector(int size)
{
    ArrangamentsResultVector_t v;
    v.size  = size;
    v.items = 0;
    v.data  = malloc(sizeof(ArrangamentsResult_t) * size);
    if (v.data != NULL)
    {
        memset(v.data, 0, sizeof(ArrangamentsResult_t) * size);
    }
    else
    {
        printf("Couldn't initialize vector.\n");
    }
    return v;
}

bool ArrangementEquals(ArrangamentsResult_t r1, llu_t strHash, int* numbers, int numbersAmount)
{
    if (r1.strHash != strHash)
        return false;
    if (r1.numbersAmount != numbersAmount)
        return false;

    return memcmp(r1.numbers, numbers, sizeof(int) * numbersAmount) == 0;
    // for (int i = 0; i < numbers.items; i++)
    //{
    //     if (r1.numbers.data[i] != numbers.data[i])
    //         return false;
    // }
    // return true;
}

int FindResult(ArrangamentsResultVector_t v, llu_t strHash, int* numbers, int numbersAmount)
{
    if (v.items > 10000)
    {
        // printf("trzeba czyscic\n");
        v.items = 0;
        return -1;
    }
    for (int i = v.items - 1; i >= 0; i--)
    {
        if (ArrangementEquals(v.data[i], strHash, numbers, numbersAmount))
        {
            return v.data[i].result;
        }
    }
    return -1;
}

void FreeArrangementVector(ArrangamentsResultVector_t* vector)
{
    // free(vector->data->numbers);
    free(vector->data);
}
// 16355956