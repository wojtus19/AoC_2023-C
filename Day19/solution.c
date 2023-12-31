#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define MAX_WORKFLOWS 11
#define MAX_RATINGS 5
#else
#define INPUT_FILE "./input.txt"
#define MAX_WORKFLOWS 508
#define MAX_RATINGS 400
#endif // EXAMPLE

#define LINE_LEN 100
#define WORKFLOW_MAX_LEN 5
#define MAX_RULES 10

typedef long long int lld_t;

typedef enum Parts_t
{
    PART_X = 0,
    PART_M = 1,
    PART_A = 2,
    PART_S = 3,
} Parts_t;

typedef struct Rule_t
{
    char part;
    char cmp;
    int value;
    char desitination[WORKFLOW_MAX_LEN];
} Rule_t;

typedef struct Workflow_t
{
    char id[WORKFLOW_MAX_LEN];
    Rule_t* rules;
    int rulesAmount;
    char finalDestination[WORKFLOW_MAX_LEN];
} Workflow_t;
typedef struct WorkflowArray_t
{
    Workflow_t* workflows;
    int itemCout;
} WorkflowArray_t;

typedef struct Range_t
{
    int min;
    int max;
} Range_t;

typedef struct XMAS_Ranges_t
{
    Range_t ranges[4];
} XMAS_Ranges_t;

WorkflowArray_t workflowArray;

Workflow_t InitWorkflow(int size)
{
    Workflow_t v;
    v.rulesAmount = 0;
    v.rules       = malloc(sizeof(Rule_t) * size);
    if (v.rules != NULL)
    {
        memset(v.rules, 0, sizeof(Rule_t) * size);
    }
    else
    {
        printf("Couldn't initialize vector.\n");
    }
    return v;
}

void AddWorkflow(Workflow_t* vector, Rule_t value)
{

    vector->rules[vector->rulesAmount] = value;
    vector->rulesAmount++;
}

void FreeWorkflow(Workflow_t* vector)
{
    free(vector->rules);
}

static int nDigits(int n)
{
    return (1 + (int)log10(n));
}

void PrintWorkflow(Workflow_t workflow)
{
    printf("WorkflowID: %s\n", workflow.id);

    for (int i = 0; i < workflow.rulesAmount; i++)
    {
        printf("if %c %c %d ->%s\n", workflow.rules[i].part, workflow.rules[i].cmp, workflow.rules[i].value, workflow.rules[i].desitination);
    }
    printf("finalDestination: %s\n\n", workflow.finalDestination);
}

Workflow_t FindWorkflow(char* id)
{
    for (int i = 0; i < workflowArray.itemCout; i++)
    {
        if (strcmp(workflowArray.workflows[i].id, id) == 0)
        {
            return workflowArray.workflows[i];
        }
    }
    return workflowArray.workflows[0];
}

bool Accepted(int x, int m, int a, int s, char* start)
{
    if (strcmp(start, "A") == 0)
    {
        return true;
    }
    if (strcmp(start, "R") == 0)
    {
        return false;
    }

    Workflow_t currWorkflow = FindWorkflow(start);

    int currentCmpValue = 0;
    for (int i = 0; i < currWorkflow.rulesAmount; i++)
    {
        switch (currWorkflow.rules[i].part)
        {
        case 'x':
            currentCmpValue = x;
            break;
        case 'm':
            currentCmpValue = m;
            break;
        case 'a':
            currentCmpValue = a;
            break;
        case 's':
            currentCmpValue = s;
            break;
        default:
            break;
        }

        switch (currWorkflow.rules[i].cmp)
        {
        case '<':
            if (currentCmpValue < currWorkflow.rules[i].value)
            {
                return Accepted(x, m, a, s, currWorkflow.rules[i].desitination);
            }
            break;
        case '>':
            if (currentCmpValue > currWorkflow.rules[i].value)
            {
                return Accepted(x, m, a, s, currWorkflow.rules[i].desitination);
            }
            break;
        default:
            break;
        }
    }
    return Accepted(x, m, a, s, currWorkflow.finalDestination);
}

lld_t CountPosibilities(XMAS_Ranges_t ranges, char* start)
{
    if (strcmp(start, "A") == 0)
    {
        lld_t ret = 1;
        for (int i = 0; i < 4; i++)
        {
            ret *= (ranges.ranges[i].max - ranges.ranges[i].min + 1);
        }
        return ret;
    }
    if (strcmp(start, "R") == 0)
    {
        return 0;
    }

    lld_t sum = 0;

    Workflow_t currentWorkflow = FindWorkflow(start);

    int rangeIdx = 0;

    for (int i = 0; i < currentWorkflow.rulesAmount; i++)
    {
        XMAS_Ranges_t cpyRanges;
        for (int i = 0; i < 4; i++)
        {
            cpyRanges.ranges[i].max = ranges.ranges[i].max;
            cpyRanges.ranges[i].min = ranges.ranges[i].min;
        }

        switch (currentWorkflow.rules[i].part)
        {
        case 'x':
            rangeIdx = PART_X;
            break;
        case 'm':
            rangeIdx = PART_M;
            break;
        case 'a':
            rangeIdx = PART_A;
            break;
        case 's':
            rangeIdx = PART_S;
            break;
        default:
            break;
        }
        if (currentWorkflow.rules[i].cmp == '<')
        {
            cpyRanges.ranges[rangeIdx].max = cpyRanges.ranges[rangeIdx].max > currentWorkflow.rules[i].value ? currentWorkflow.rules[i].value : cpyRanges.ranges[rangeIdx].max;
            cpyRanges.ranges[rangeIdx].max--;

            ranges.ranges[rangeIdx].min = cpyRanges.ranges[rangeIdx].max + 1;
        }
        else // if (currentWorkflow.rules[i].cmp == '>')
        {
            cpyRanges.ranges[rangeIdx].min = cpyRanges.ranges[rangeIdx].min < currentWorkflow.rules[i].value ? currentWorkflow.rules[i].value : cpyRanges.ranges[rangeIdx].min;
            cpyRanges.ranges[rangeIdx].min++;

            ranges.ranges[rangeIdx].max = cpyRanges.ranges[rangeIdx].min - 1;
        }
        sum += CountPosibilities(cpyRanges, currentWorkflow.rules[i].desitination);
    }
    sum += CountPosibilities(ranges, currentWorkflow.finalDestination);

    return sum;
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

    workflowArray.workflows = (Workflow_t*)malloc(sizeof(Workflow_t) * MAX_WORKFLOWS);
    workflowArray.itemCout  = 0;

    char wf[WORKFLOW_MAX_LEN];
    char part;
    char cmp;
    int val;
    char workflowDest[WORKFLOW_MAX_LEN];
    do
    {
        fgets(line, sizeof(line), pInputFile);

        if (strcmp(line, "\n") == 0)
            continue;

        memset(wf, 0, sizeof(wf));
        sscanf(line, "%[^{]s", wf);

        Workflow_t newWorkFlow = InitWorkflow(MAX_RULES);
        memcpy(newWorkFlow.id, wf, sizeof(newWorkFlow.id));

        int idx = strlen(wf) + 1;
        while (line[idx] != '}')
        {
            memset(workflowDest, 0, sizeof(workflowDest));
            part = line[idx];

            if ((part == 'x' || part == 'm' || part == 'a' || part == 's') && (line[idx + 1] == '<' || line[idx + 1] == '>'))
            {
                idx++;
                cmp = line[idx];
                idx++;
                val = atoi(&line[idx]);
                idx += nDigits(val) + 1;
                sscanf(&line[idx], "%[^:,}]s", workflowDest);

                idx += strlen(workflowDest) + 1;

                Rule_t newRule;
                newRule.cmp   = cmp;
                newRule.part  = part;
                newRule.value = val;
                memcpy(newRule.desitination, workflowDest, sizeof(newRule.desitination));

                AddWorkflow(&newWorkFlow, newRule);
            }
            else
            {
                sscanf(&line[idx], "%[^:,}]s", workflowDest);
                idx += strlen(workflowDest);
                memcpy(newWorkFlow.finalDestination, workflowDest, sizeof(newWorkFlow.finalDestination));
            }
        }
        workflowArray.workflows[workflowArray.itemCout] = newWorkFlow;
        workflowArray.itemCout++;
    } while (strcmp(line, "\n") != 0);

    lld_t accepted = 0;
    int x, m, a, s;

    while (fgets(line, sizeof(line), pInputFile) != 0)
    {
        sscanf(line, "{x=%d,m=%d,a=%d,s=%d}", &x, &m, &a, &s);
        if (Accepted(x, m, a, s, "in"))
        {
            accepted += x + m + a + s;
        }
    }

    printf("accepted: %lld\n", accepted);

    XMAS_Ranges_t start;
    for (int i = 0; i < 4; i++)
    {
        start.ranges[i].min = 1;
        start.ranges[i].max = 4000;
    }

    lld_t possibilites = CountPosibilities(start, "in");

    printf("possibilites: %lld\n", possibilites);

    for (int i = 0; i < workflowArray.itemCout; i++)
    {
        FreeWorkflow(&workflowArray.workflows[i]);
    }
    free(workflowArray.workflows);

    return 0;
}