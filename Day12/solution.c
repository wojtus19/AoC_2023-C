#include "Vector.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define LINE_LEN 25
#define INPUT_FILE "./example.txt"
#define ROWS 6
#else
#define LINE_LEN 250
#define INPUT_FILE "./input.txt"
#define ROWS 1000
#endif
#define START_NUMBERS 21

#define INIT_SIZE 8

bool IsVectorEqual(Vector_t v1, Vector_t v2)
{
    if (v1.items != v2.items)
        return false;

    return memcmp(v1.data, v2.data, sizeof(int) * v1.items) == 0;
}

bool IsArrangementValid(Vector_t line, Vector_t rowNumbers)
{
    int n = line.items;

    Vector_t runs = InitVector(INIT_SIZE);

    int i = 0;
    while (i < n)
    {
        while (i < n && !line.data[i])
        {
            i++;
        }
        if (i == n)
            break;
        int j = i;
        int c = 0;
        while (j < n && line.data[j] != 0)
        {
            j++;
            c++;
        }
        AddToVector(&runs, c);
        i = j;
    }

    bool valid = IsVectorEqual(runs, rowNumbers);
    FreeVector(&runs);

    return valid;
}

int CalculateArrangements(char* rowString, Vector_t rowNumbers)
{
    Vector_t line = InitVector(INIT_SIZE);
    Vector_t idxs = InitVector(INIT_SIZE);

    for (int i = 0; i < strlen(rowString); i++)
    {
        if (rowString[i] == '.')
        {
            AddToVector(&line, 0);
        }
        else if (rowString[i] == '?')
        {
            AddToVector(&line, -1);
            AddToVector(&idxs, i);
        }
        else if (rowString[i] == '#')
        {
            AddToVector(&line, 1);
        }
    }

    int arrangements = 0;
    for (unsigned int mask = 1; mask <= pow(2, idxs.items); mask++)
    {
        Vector_t lineCopy = CopyVector(line);

        for (int idx = 0; idx < idxs.items; idx++)
        {
            if (mask & (1 << idx))
            {
                lineCopy.data[idxs.data[idx]] = 0;
            }
            else
            {
                lineCopy.data[idxs.data[idx]] = 1;
            }
        }

        if (IsArrangementValid(lineCopy, rowNumbers))
        {
            arrangements++;
        }
        FreeVector(&lineCopy);
    }

    FreeVector(&line);
    FreeVector(&idxs);
    return arrangements;
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
    int sum = 0;

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

        char* rowString = (char*)malloc(sizeof(char) * cnt + 1);
        memset(rowString, 0, sizeof(char) * cnt + 1);
        strncpy(rowString, line, cnt);

        p = p + cnt + 1;

        while (true)
        {
            int tmp = strtol(p, &p, 0);
            p++;
            if (tmp == 0)
                break;
            AddToVector(&rowNumbers, tmp);
        }

        int arrangements = CalculateArrangements(rowString, rowNumbers);

        // printf("%d.  arrangements: %llu\n", idx + 1, arrangements);

        sum += arrangements;
        free(rowString);
        FreeVector(&rowNumbers);
    }

    printf("sum: %d\n", sum);

    return 0;
}