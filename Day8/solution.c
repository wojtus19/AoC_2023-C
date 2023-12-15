#include "list.h"
#include <stdio.h>
#include <string.h>

#ifndef DATA_SIZE
#define DATA_SIZE 3
#endif // !DATA_SIZE
#include <stdbool.h>

#define INPUT_FILE "./input.txt"
#define COMMAND_FILE "./commands.txt"
#define COMMAND_AMOUNT 307
#define LINE_LEN 18
#define LINES 714

#define STARTS_NUM 6
#define STOPS_NUM 6
#define LEFT_COMMAND 'L'
#define RIGHT_COMMAND 'R'

typedef long long unsigned int llu_t;

llu_t calculateGCD(llu_t n1, llu_t n2)
{
    if (n2 == 0)
        return n1;
    else
        return calculateGCD(n2, n1 % n2);
}

llu_t calculateLCM(llu_t n1, llu_t n2)
{
    llu_t gcd = calculateGCD(n1, n2);
    return (n1 * n2) / gcd;
}

int main()
{

    FILE* pInputFile;
    FILE* pCommandFile;

    pInputFile = fopen(INPUT_FILE, "r");
    if (NULL == pInputFile)
    {
        printf("file cannot be opened\n");
        return -1;
    }

    pCommandFile = fopen(COMMAND_FILE, "r");
    if (NULL == pCommandFile)
    {
        printf("file cannot be opened\n");
        return -1;
    }

    char commands[COMMAND_AMOUNT + 2] = { 0 };
    if (fgets(commands, sizeof(commands), pCommandFile) == 0)
    {
        return -1;
    }

    char line[LINE_LEN + 2];
    char nodes[LINES][DATA_SIZE + 1];
    char leftNodes[LINES][DATA_SIZE + 1];
    char rightNodes[LINES][DATA_SIZE + 1];
    List_t* list               = NULL;
    int idx                    = 0;
    List_t* starts[STARTS_NUM] = { 0 };
    List_t* stops[STARTS_NUM]  = { 0 };
    int startIdx               = 0;
    int stopIdx                = 0;

    while (fgets(line, sizeof(line), pInputFile) != 0)
    {
        sscanf(line, "%3c = (%3c, %3c)", nodes[idx], leftNodes[idx], rightNodes[idx]);
        nodes[idx][DATA_SIZE]      = '\0';
        leftNodes[idx][DATA_SIZE]  = '\0';
        rightNodes[idx][DATA_SIZE] = '\0';

        idx++;
    }

    for (int i = 0; i < LINES; i++)
    {
        Push(&list, nodes[i]);
    }
    for (int i = 0; i < LINES; i++)
    {
        AddLeft(list, nodes[i], leftNodes[i]);
        AddRight(list, nodes[i], rightNodes[i]);
        if (nodes[i][2] == 'A')
        {
            starts[startIdx] = Find(list, nodes[i]);
            startIdx++;
        }
        else if (nodes[i][2] == 'Z')
        {
            stops[stopIdx] = Find(list, nodes[i]);
            stopIdx++;
        }
    }

    llu_t partialNumbers[STARTS_NUM] = { 0 };
    for (int idx = 0; idx < STARTS_NUM; idx++)
    {
        llu_t count = 1;
        while (true)
        {
            for (int i = 0; i < COMMAND_AMOUNT; i++)
            {
                if (commands[i] == LEFT_COMMAND)
                {
                    starts[idx] = starts[idx]->left;
                }
                if (commands[i] == RIGHT_COMMAND)
                {
                    starts[idx] = starts[idx]->right;
                }
                for (int k = 0; k < STOPS_NUM; k++)
                {
                    if (strcmp(starts[idx]->data, stops[k]->data) == 0)
                    {
                        goto end;
                    }
                }
                count++;
            }
        }
    end:
        partialNumbers[idx] = count;
        count               = 0;
    }

    llu_t LCM = partialNumbers[0];
    for (int i = 1; i < STARTS_NUM; i++)
    {
        LCM = calculateLCM(LCM, partialNumbers[i]);
        // 11188774513823
    }

    printf("LCM: %llu\n", LCM);

    return 0;
}