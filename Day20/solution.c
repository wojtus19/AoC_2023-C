#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#else
#define INPUT_FILE "./input.txt"
#endif // EXAMPLE

#define LINE_LEN 100
#define WORKFLOW_MAX_LEN 5
#define MAX_RULES 10
#define INIT_SIZE 58
#define MODULE_ID_MAX_SIZE 15
#define MAX_DESITNATION_AMOUNT 10

#define HIGH true
#define LOW false

#define MAX_PULSES_MEMORY 20
#define Q_SIZE 1000

typedef bool pulse_t;

typedef enum ModuleType_t
{
    BROADCAST   = 'B',
    FLIP_FLOP   = '%',
    CONJUNCTION = '&',
} ModuleType_t;

typedef struct PulseMemory_t
{
    char from[MODULE_ID_MAX_SIZE];
    pulse_t pulse;
} PulseMemory_t;

typedef struct Module_t
{
    ModuleType_t type;
    char id[MODULE_ID_MAX_SIZE];
    char (*destinationModulesIds)[MODULE_ID_MAX_SIZE];
    int destinationModulesAmount;
    PulseMemory_t memory[MAX_PULSES_MEMORY];
    int memoryIdx;
} Module_t;

typedef struct ModuleVector_t
{
    Module_t* modules;
    int modulesAmount;
    int vectorSize;
} ModuleVector_t;

typedef struct PulseSend_t
{
    char destination[MODULE_ID_MAX_SIZE];
    char source[MODULE_ID_MAX_SIZE];
    pulse_t pulse;
} PulseSend_t;

Module_t InitModule(ModuleType_t type, const char* id)
{
    Module_t m;
    m.destinationModulesAmount = 0;
    m.type                     = type;
    m.memoryIdx                = 0;
    memset(m.memory, LOW, sizeof(m.memory));
    strncpy(m.id, id, sizeof(m.id));
    m.destinationModulesIds = malloc(sizeof(*m.destinationModulesIds) * MAX_DESITNATION_AMOUNT);
    if (m.destinationModulesIds != NULL)
    {
        memset(m.destinationModulesIds, 0, sizeof(*m.destinationModulesIds) * MAX_DESITNATION_AMOUNT);
    }
    else
    {
        printf("Couldn't initialize Module");
    }
    return m;
}

void AddModule(Module_t* m, const char* id)
{
    strncpy(m->destinationModulesIds[m->destinationModulesAmount], id, strlen(id));
    m->destinationModulesAmount++;
}

void FreeModule(Module_t* m)
{
    free(m->destinationModulesIds);
}

void PrintModule(Module_t m)
{
    printf("type: %c, id: %s -> ", m.type, m.id);
    for (int i = 0; i < m.destinationModulesAmount; i++)
    {

        printf("%s, ", m.destinationModulesIds[i]);
    }

    if (m.type == CONJUNCTION)
    {
        printf("| memory: ");
        for (int i = 0; i < m.memoryIdx; i++)
        {
            printf("{ %s: %d } ", m.memory[i].from, m.memory[i].pulse);
        }
    }
    else
    {
        printf("| memory: %d", m.memory[0].pulse);
    }
    printf("\n");
}

ModuleVector_t InitModulesVector(int size)
{
    ModuleVector_t mv;
    mv.vectorSize    = size;
    mv.modulesAmount = 0;
    mv.modules       = (Module_t*)malloc(sizeof(Module_t) * mv.vectorSize);

    return mv;
}

void AddModuleToVector(ModuleVector_t* v, Module_t m)
{
    if (v->modulesAmount >= v->vectorSize)
    {
        v->vectorSize *= 2;
        v->modules = realloc(v->modules, sizeof(Module_t) * v->vectorSize);
    }
    v->modules[v->modulesAmount] = m;
    v->modulesAmount++;
}

void PrintModulesVector(ModuleVector_t v)
{
    for (int i = 0; i < v.modulesAmount; i++)
    {
        PrintModule(v.modules[i]);
    }
}

void FreeModulesVector(ModuleVector_t* v)
{

    for (int i = 0; i < v->modulesAmount; i++)
    {
        FreeModule(&v->modules[i]);
    }

    free(v->modules);
}

int FindModule(ModuleVector_t v, char* id)
{
    for (int i = 0; i < v.modulesAmount; i++)
    {
        if (strcmp(id, v.modules[i].id) == 0)
            return i;
    }
    // printf("did not find: %s\n", id);
    return -1;
}

bool IsInDestination(Module_t where, Module_t what)
{
    for (int i = 0; i < where.destinationModulesAmount; i++)
    {
        if (strcmp(where.destinationModulesIds[i], what.id) == 0)
            return true;
    }
    return false;
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

    char broadcaster[]              = "broadcaster";
    char id[MODULE_ID_MAX_SIZE]     = { 0 };
    char destId[MODULE_ID_MAX_SIZE] = { 0 };

    ModuleVector_t modulesVector = InitModulesVector(INIT_SIZE);

    int broadcasterIdx = 0;

    while (fgets(line, sizeof(line), pInputFile))
    {
        Module_t newModule;
        ModuleType_t mType;

        if (strncmp(line, broadcaster, sizeof(broadcaster - 1)) == 0)
        {
            strcpy(id, broadcaster);
            mType = BROADCAST;
            // printf("broadcaster: %s\n", line);
        }
        else if (line[0] == '%')
        {
            sscanf(&line[1], "%[^ ]s", id);
            mType = FLIP_FLOP;
            // printf("Flip-flop: %s-\n", line);
        }
        else if (line[0] == '&')
        {
            sscanf(&line[1], "%[^ ]s", id);
            mType = CONJUNCTION;
            // printf("Conjunction: %s-\n", line);
        }
        // printf("id: %s\n", id);
        newModule = InitModule(mType, id);

        char* pDestStr = strrchr(line, '>');
        pDestStr += 2;
        // printf("%s-%lld\n", pDestStr, strlen(pDestStr));

        while (strlen(pDestStr) > 1)
        {
            memset(destId, 0, sizeof(destId));
            sscanf(pDestStr, "%[^ ,\n]s", destId);
            if (strlen(destId) > 0)
            {
                AddModule(&newModule, destId);
            }
            pDestStr += strlen(destId) + 1;
        }
        if (newModule.type == BROADCAST)
        {
            broadcasterIdx = modulesVector.modulesAmount;
        }
        AddModuleToVector(&modulesVector, newModule);
    }

    for (int i = 0; i < modulesVector.modulesAmount; i++)
    {
        if (modulesVector.modules[i].type == CONJUNCTION)
        {
            for (int j = 0; j < modulesVector.modulesAmount; j++)
            {
                if (IsInDestination(modulesVector.modules[j], modulesVector.modules[i]))
                {
                    strncpy(modulesVector.modules[i].memory[modulesVector.modules[i].memoryIdx].from, modulesVector.modules[j].id, sizeof(modulesVector.modules[j].id));
                    modulesVector.modules[i].memory[modulesVector.modules[i].memoryIdx].pulse = LOW;
                    modulesVector.modules[i].memoryIdx++;
                }
            }
        }
    }

    // PrintModulesVector(modulesVector);

    PulseSend_t q[Q_SIZE];
    int qCurrIdx = 0;
    int qAmount  = 0;

    int lowPulseCount  = 0;
    int highPulseCount = 0;

    for (int i = 0; i < 1000; i++)
    {
        lowPulseCount++;
        for (int j = 0; j < modulesVector.modules[broadcasterIdx].destinationModulesAmount; j++)
        {
            strncpy(q[qAmount].destination, modulesVector.modules[broadcasterIdx].destinationModulesIds[j], sizeof(q[qAmount].destination));
            q[qAmount].pulse = LOW;
            lowPulseCount++;
            qAmount++;
        }
        while (qAmount != qCurrIdx)
        {
            int currModuleIdx = FindModule(modulesVector, q[qCurrIdx].destination);
            if (currModuleIdx == -1)
            {
                qCurrIdx++;
                continue;
            }
            if (modulesVector.modules[currModuleIdx].type == FLIP_FLOP) // %
            {
                if (q[qCurrIdx].pulse == LOW)
                {
                    modulesVector.modules[currModuleIdx].memory[0].pulse = !modulesVector.modules[currModuleIdx].memory[0].pulse;
                    for (int j = 0; j < modulesVector.modules[currModuleIdx].destinationModulesAmount; j++)
                    {
                        strncpy(q[qAmount].destination, modulesVector.modules[currModuleIdx].destinationModulesIds[j], sizeof(q[qAmount].destination));
                        strncpy(q[qAmount].source, modulesVector.modules[currModuleIdx].id, sizeof(q[qAmount].destination));
                        q[qAmount].pulse = modulesVector.modules[currModuleIdx].memory[0].pulse;
                        if (q[qAmount].pulse == HIGH)
                        {
                            highPulseCount++;
                        }
                        else
                        {
                            lowPulseCount++;
                        }
                        qAmount++;
                    }
                }
                qCurrIdx++;
            }
            else // & - CONJUNCTION
            {
                for (int j = 0; j < modulesVector.modules[currModuleIdx].memoryIdx; j++)
                {
                    if (strcmp(modulesVector.modules[currModuleIdx].memory[j].from, q[qCurrIdx].source) == 0)
                    {
                        modulesVector.modules[currModuleIdx].memory[j].pulse = q[qCurrIdx].pulse;
                    }
                }
                pulse_t sendPulse = LOW;
                for (int j = 0; j < modulesVector.modules[currModuleIdx].memoryIdx; j++)
                {
                    if (modulesVector.modules[currModuleIdx].memory[j].pulse == LOW)
                    {
                        sendPulse = HIGH;
                        break;
                    }
                }
                for (int j = 0; j < modulesVector.modules[currModuleIdx].destinationModulesAmount; j++)
                {
                    strncpy(q[qAmount].destination, modulesVector.modules[currModuleIdx].destinationModulesIds[j], sizeof(q[qAmount].destination));
                    strncpy(q[qAmount].source, modulesVector.modules[currModuleIdx].id, sizeof(q[qAmount].destination));
                    q[qAmount].pulse = sendPulse;
                    if (q[qAmount].pulse == HIGH)
                    {
                        highPulseCount++;
                    }
                    else
                    {
                        lowPulseCount++;
                    }

                    qAmount++;
                }
                qCurrIdx++;
            }
        }
        qCurrIdx = 0;
        qAmount  = 0;
        memset(q, 0, sizeof(q));
    }

    printf("low: %d, high: %d\n", lowPulseCount, highPulseCount);
    printf("ans: %d\n", lowPulseCount * highPulseCount);

    FreeModulesVector(&modulesVector);
    return 0;
}
