#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define LINE_LEN 18
#define INPUT_FILE "./example.txt"
#define START_NUMBERS 6
#define ROWS 3
#else
#define LINE_LEN 125
#define INPUT_FILE "./input.txt"
#define START_NUMBERS 21
#define ROWS 200
#endif

bool AllZeroes(int array[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (array[i] != 0)
            return false;
    }
    return true;
}

void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int CalculateExtrapolationForward(int numbers[], int size)
{

    int* numbersCopy = malloc(sizeof(int) * (size + 1));
    memset(numbersCopy, 0, sizeof(int) * (size + 1));
    memcpy(numbersCopy, numbers, sizeof(int) * size);

    int* secondHistory = malloc(sizeof(int) * size);

    if (!AllZeroes(numbersCopy, size + 1))
    {
        for (int i = 0; i < size - 1; i++)
        {
            secondHistory[i] = numbers[i + 1] - numbers[i];
        }
        secondHistory[size - 1] = CalculateExtrapolationForward(secondHistory, size - 1);
    }
    else
    {
        free(secondHistory);
        free(numbersCopy);
        return 0;
    }
    int retVal = secondHistory[size - 1] + numbersCopy[size - 1];
    free(secondHistory);
    free(numbersCopy);
    return retVal;
}

int CalculateExtrapolationBackwards(int numbers[], int size)
{
    int* numbersCopy = malloc(sizeof(int) * (size + 1));
    memset(numbersCopy, 0, sizeof(int) * (size + 1));
    memcpy(&numbersCopy[1], numbers, sizeof(int) * size);
    int* secondHistory = malloc(sizeof(int) * size);

    if (!AllZeroes(numbersCopy, size + 1))
    {
        for (int i = size; i > 0; i--)
        {
            secondHistory[i - 1] = numbers[i] - numbers[i - 1];
        }
        secondHistory[0] = CalculateExtrapolationBackwards(secondHistory, size - 1);
    }
    else
    {
        free(secondHistory);
        free(numbersCopy);
        return 0;
    }
    // printArray(numbersCopy, size + 1);
    int retVal = numbersCopy[1] - secondHistory[0];
    free(secondHistory);
    free(numbersCopy);
    return retVal;
}

int main()
{

    FILE* pInputFile;
    char line[LINE_LEN + 2];

    pInputFile = fopen(INPUT_FILE, "r");
    if (NULL == pInputFile)
    {
        printf("file cannot be opened\n");
        return -1;
    }

    int numbers[ROWS][START_NUMBERS];
    char* p;

    for (int idx = 0; idx < ROWS; idx++)
    {
        p = fgets(line, sizeof(line), pInputFile);

        for (int j = 0; j < START_NUMBERS; j++)
        {
            numbers[idx][j] = strtol(p, &p, 0);
        }
    }

    int sumForwards  = 0;
    int sumBackwards = 0;

    for (int idx = 0; idx < ROWS; idx++)
    {
        sumForwards += CalculateExtrapolationForward(numbers[idx], START_NUMBERS);
        sumBackwards += CalculateExtrapolationBackwards(numbers[idx], START_NUMBERS);
    }

    printf("sumForwards: %d\nsumBackawrds: %d\n", sumForwards, sumBackwards);

    return 0;
}