
#include "Vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define LINE_LEN 500
#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#else
#define INPUT_FILE "./input.txt"
#endif

#define MAP_NUMBER 7
#define INIT_SIZE 60

#define NO_THREADS 10

llu globalMinLocation = __LONG_LONG_MAX__;

typedef struct Params_t
{
    llu seedStart;
    llu howMany;
    Vector_t* mapDsc;
    Vector_t* mapSrc;
    Vector_t* mapRange;
} Params_t;

CRITICAL_SECTION cs;

llu FindLocation(llu seed, Vector_t mapDsc[], Vector_t mapSrc[], Vector_t mapRange[])
{
    llu currNum = seed;
    for (int mapIdx = 0; mapIdx < MAP_NUMBER; mapIdx++)
    {
        for (int mapPartIdx = 0; mapPartIdx < mapDsc[mapIdx].items; mapPartIdx++)
        {
            if (currNum >= mapSrc[mapIdx].data[mapPartIdx] && currNum < (mapSrc[mapIdx].data[mapPartIdx] + mapRange[mapIdx].data[mapPartIdx]))
            {
                currNum = mapDsc[mapIdx].data[mapPartIdx] + (currNum - mapSrc[mapIdx].data[mapPartIdx]);
                break;
            }
        }
    }
    return currNum;
}

DWORD WINAPI FindLocationFromRange(LPVOID params)
{
    clock_t begin          = clock();
    Params_t* paramsStruct = (Params_t*)params;

    llu seedStart      = paramsStruct->seedStart;
    llu howMany        = paramsStruct->howMany;
    Vector_t* mapDsc   = paramsStruct->mapDsc;
    Vector_t* mapSrc   = paramsStruct->mapSrc;
    Vector_t* mapRange = paramsStruct->mapRange;
    llu seed           = seedStart;
    llu min            = FindLocation(seedStart, mapDsc, mapSrc, mapRange);
    for (llu i = 1; i < howMany; i++)
    {
        seed += (llu)1;
        llu currNum = FindLocation(seed, mapDsc, mapSrc, mapRange);
        if (currNum < min)
        {
            min = currNum;
        }
    }
    clock_t end      = clock();
    double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
    EnterCriticalSection(&cs);
    printf("Entering Critical Section (time spent: %f), min: %lld\n", timeSpent, min);
    if (min < globalMinLocation)
    {
        globalMinLocation = min;
        printf("New global min: %lld\n", globalMinLocation);
    }

    LeaveCriticalSection(&cs);
    return 0;
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

    Vector_t seeds = InitVector(INIT_SIZE);

    char* p;
    p = fgets(line, sizeof(line), pInputFile);

    while (true)
    {
        llu tmp = strtoll(p, &p, 0);
        p++;
        if (tmp != 0)
        {
            AddToVector(&seeds, tmp);
        }
        if (*p == '\0')
            break;
    }

    Vector_t mapsDst[MAP_NUMBER];
    Vector_t mapsSrc[MAP_NUMBER];
    Vector_t mapsRange[MAP_NUMBER];
    for (int i = 0; i < MAP_NUMBER; i++)
    {
        mapsDst[i]   = InitVector(INIT_SIZE);
        mapsSrc[i]   = InitVector(INIT_SIZE);
        mapsRange[i] = InitVector(INIT_SIZE);
    }
    p = fgets(line, sizeof(line), pInputFile);

    for (int mapIdx = 0; mapIdx < MAP_NUMBER; mapIdx++)
    {
        p = fgets(line, sizeof(line), pInputFile);
        while (true)
        {
            p = fgets(line, sizeof(line), pInputFile);

            if (strlen(p) < 4)
            {
                break;
            }
            llu dsc;
            llu src;
            llu rng;
            sscanf(line, "%llu %llu %llu\n", &dsc, &src, &rng);
            AddToVector(&mapsDst[mapIdx], dsc);
            AddToVector(&mapsSrc[mapIdx], src);
            AddToVector(&mapsRange[mapIdx], rng);
        }
    }

    llu minLocation = FindLocation(seeds.data[0], mapsDst, mapsSrc, mapsRange);
    for (int seedIdx = 1; seedIdx < seeds.items; seedIdx++)
    {
        llu location = FindLocation(seeds.data[seedIdx], mapsDst, mapsSrc, mapsRange);
        if (location < minLocation)
        {
            minLocation = location;
        }
    }
    printf("minLocation: %lld\n", minLocation);

    HANDLE hThread[NO_THREADS];
    DWORD dwThreadId[NO_THREADS];

    InitializeCriticalSection(&cs);

    int threadIdx = 0;
    Params_t params[NO_THREADS];
    for (int seedIdx = 0; seedIdx < seeds.items - 1; seedIdx += 2)
    {
        params[threadIdx].seedStart = seeds.data[seedIdx];
        params[threadIdx].howMany   = seeds.data[seedIdx + 1];
        params[threadIdx].mapDsc    = mapsDst;
        params[threadIdx].mapSrc    = mapsSrc;
        params[threadIdx].mapRange  = mapsRange;

        hThread[threadIdx] = CreateThread(NULL, 0, FindLocationFromRange, &params[threadIdx], 0, &dwThreadId[threadIdx]);
        threadIdx++;
    }

    WaitForMultipleObjects(NO_THREADS, hThread, TRUE, INFINITE);
    DeleteCriticalSection(&cs);
    for (int i = 0; i < threadIdx; i++)
    {
        CloseHandle(hThread[i]);
    }

    printf("global Min: %lld\n", globalMinLocation);

    return 0;
}