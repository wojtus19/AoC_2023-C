#include "Vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define ROWS 6
#else
#define INPUT_FILE "./input.txt"
#define ROWS 1000
#endif               // EXAMPLE
#define LINE_LEN 250 // 18

#define INIT_SIZE 8
#define MEMORY_SIZE 1000

#define MEM_MAX 0xFFFFFFFFFFFFFFFF
typedef struct memo_t
{
    int numbersAmount;
    int* numbers;
    char* s;
    llu_t result;
} memo_t;

memo_t memory[MEMORY_SIZE];

int Hash(const char* str)
{
    unsigned long hash = 317;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + (c * 2179);

    return hash % MEMORY_SIZE;
}

bool CharacterPresent(const char* str, char c, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (str[i] == c)
            return true;
    }
    return false;
}

bool IsInMemory(unsigned hash, const char* s, const int* n, int amount)
{
    if (memory[hash].result < MEM_MAX)
    {
        if (strcmp(memory[hash].s, s) != 0)
            return false;
        if (memory[hash].numbersAmount != amount)
            return false;
        for (int i = 0; i < amount; i++)
        {
            if (n[i] != memory[hash].numbers[i])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

llu_t CalculateArrangementsRecursive(const char* rowString, const int* numbers, int numbersAmount)
{
    int sLen = strlen(rowString);
    if (sLen == 0)
    {
        if (numbersAmount == 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    if (numbersAmount == 0)
    {
        return CharacterPresent(rowString, '#', strlen(rowString)) ? 0 : 1;
    }

    unsigned hash = Hash(rowString);

    bool inMemory = IsInMemory(hash, rowString, numbers, numbersAmount);
    if (inMemory)
    {
        return memory[hash].result;
    }

    llu_t result = 0;

    if (rowString[0] == '.' || rowString[0] == '?')
    {
        result += CalculateArrangementsRecursive(&rowString[1], numbers, numbersAmount);
    }
    if (rowString[0] == '#' || rowString[0] == '?')
    {
        if (numbers[0] <= sLen && !CharacterPresent(rowString, '.', numbers[0]) && (numbers[0] == sLen || (numbers[0] < sLen && rowString[numbers[0]] != '#')))
        {
            if (numbers[0] + 1 >= sLen)
            {
                result += CalculateArrangementsRecursive("", &numbers[1], numbersAmount - 1);
            }
            else
            {
                result += CalculateArrangementsRecursive(&rowString[numbers[0] + 1], &numbers[1], numbersAmount - 1);
            }
        }
    }
    memo_t res;
    int* numCopy = (int*)malloc(sizeof(int) * numbersAmount);
    memcpy(numCopy, numbers, sizeof(int) * numbersAmount);

    char* strCopy = (char*)malloc(sizeof(char) * (sLen + 1));
    memset(strCopy, '\0', sizeof(char) * (sLen + 1));
    strcpy(strCopy, rowString);
    res.numbers       = numCopy;
    res.s             = strCopy;
    res.result        = result;
    res.numbersAmount = numbersAmount;

    memory[hash] = res;

    return result;
}

void FreeMemory()
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (memory[i].result < MEM_MAX)
        {
            free(memory[i].numbers);
            free(memory[i].s);
        }
    }
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
    char* p;
    llu_t sum = 0;

    memset(memory, 0xFF, sizeof(memory));

    for (int idx = 0; idx < ROWS; idx++)
    {

        Vector_t rowNumbers = InitVector(INIT_SIZE);
        p                   = fgets(line, sizeof(line), pInputFile);
        if (NULL == p)
            return -1;

        int cnt = 0;
        while (line[cnt] != ' ')
        {
            cnt++;
        }

        int CopiesNumber = 5;
        char* rowString  = (char*)malloc(sizeof(char) * ((cnt + 1) * CopiesNumber));
        memset(rowString, 0, sizeof(char) * ((cnt + 1) * CopiesNumber));

        for (int i = 0; i < CopiesNumber; i++)
        {
            strncpy(&rowString[(cnt + 1) * i], line, cnt);
            if (i < CopiesNumber - 1)
            {
                strcat(rowString, "?");
            }
        }

        p = p + cnt + 1;

        while (true)
        {
            int tmp = strtol(p, &p, 0);
            p++;
            if (tmp == 0)
                break;
            AddToVector(&rowNumbers, tmp);
        }

        int itemCount = rowNumbers.items;
        int* numbers  = (int*)malloc(sizeof(int) * CopiesNumber * rowNumbers.items);
        memset(numbers, 0, sizeof(int) * CopiesNumber * rowNumbers.items);

        for (int x = 0; x < CopiesNumber; x++)
        {
            for (int i = 0; i < itemCount; i++)
            {
                numbers[x * itemCount + i] = rowNumbers.data[i];
            }
        }

        llu_t arrangements = (llu_t)CalculateArrangementsRecursive(rowString, numbers, CopiesNumber * rowNumbers.items);

        sum += arrangements;

        free(rowString);
        free(numbers);
        FreeVector(&rowNumbers);

        FreeMemory();
        memset(memory, 0xFF, sizeof(memory));
    }

    printf("sum: %llu\n", sum);

    return 0;
}