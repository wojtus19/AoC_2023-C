#include "Vector.h"
#include <stdio.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#else
#define INPUT_FILE "./input.txt"
#endif // EXAMPLE

#define LINE_LEN 15

#ifndef MAX_STR
#define MAX_STR 10
#endif // !MAX_STR

#define MODULO 256

#define INIT_SIZE 128

int Hash(char* s, int len)
{
    int hash = 0;
    for (int i = 0; i < len; i++)
    {
        hash += s[i];
        hash *= 17;
        hash = hash % MODULO;
    }

    return hash;
}

char GetCommand(char* s, int len)
{

    for (int i = 0; i < len; i++)
    {
        if (s[i] == '-')
            return s[i];
        if (s[i] == '=')
            return s[i];
    }
    return '\0';
}

void GetLabel(char label[MAX_STR], char* fromStr, int len)
{
    int idx = 0;
    while (fromStr[idx] >= 'a' && fromStr[idx] <= 'z')
    {
        label[idx] = fromStr[idx];
        idx++;
    }
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

    Vector_t boxes[MODULO];
    for (int i = 0; i < MODULO; i++)
    {
        boxes[i] = InitVector(INIT_SIZE);
    }

    while (fgets(line, sizeof(line), pInputFile) != 0)
    {
        char label[MAX_STR] = { 0 };

        GetLabel(label, line, strlen(line) - 1);

        int Idx      = Hash(label, strlen(label));
        char command = GetCommand(line, strlen(line) - 1);
        if (command == '=')
        {
            int value = 0;
            sscanf(line, "%*[^=]=%d", &value);
            AddToVector(&boxes[Idx], label, value);
        }
        else if (command == '-')
        {
            DeleteFromVector(&boxes[Idx], label);
        }
    }

    int sum = 0;

    for (int i = 0; i < MODULO; i++)
    {
        if (boxes[i].items > 0)
        {

            for (int j = 0; j < boxes[i].items; j++)
            {
                sum += (i + 1) * (j + 1) * boxes[i].data[j].value;
            }

            // printf("%d. ", i);
            // PrintVector(boxes[i]);
        }
    }

    for (int i = 0; i < MODULO; i++)
    {
        FreeVector(&boxes[i]);
    }

    printf("sum: %d\n", sum);
    return 0;
}