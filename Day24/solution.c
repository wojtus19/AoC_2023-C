#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define HAILSTONES_NUM 5
#else
#define INPUT_FILE "./input.txt"
#define HAILSTONES_NUM 300
#endif // EXAMPLE

#define LINE_LEN 100

typedef long long int lld_t;

typedef struct Coords_t
{
    lld_t x;
    lld_t y;
    lld_t z;
} Coords_t;

typedef struct Hailstone_t
{
    Coords_t position;
    Coords_t velocity;
} Hailstone_t;

void PrintOneHailstone(Hailstone_t h)
{
    printf("%lld, %lld, %lld @ %lld, %lld, %lld\n", h.position.x, h.position.y, h.position.z, h.velocity.x, h.velocity.y, h.velocity.z);
}

void PrintHailstones(Hailstone_t* hailstones, int size)
{
    for (int i = 0; i < size; i++)
    {
        PrintOneHailstone(hailstones[i]);
    }
    printf("\n");
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

    Hailstone_t hailstones[HAILSTONES_NUM];

    for (int i = 0; i < HAILSTONES_NUM; i++)
    {
        fgets(line, sizeof(line), pInputFile);

        sscanf(line, "%lld, %lld, %lld @ %lld, %lld, %lld\n", &hailstones[i].position.x, &hailstones[i].position.y, &hailstones[i].position.z, &hailstones[i].velocity.x, &hailstones[i].velocity.y, &hailstones[i].velocity.z);
    }

    // PrintHailstones(hailstones, HAILSTONES_NUM);

    // lld_t min = 7;
    // lld_t max = 27;

    lld_t min = 200000000000000;
    lld_t max = 400000000000000;
    lld_t sum = 0;
    for (int i = 0; i < HAILSTONES_NUM; i++)
    {
        for (int j = i + 1; j < HAILSTONES_NUM; j++)
        {
            long double ai = hailstones[i].velocity.y;
            long double bi = (-1 * hailstones[i].velocity.x);
            long double ci = (hailstones[i].velocity.y * hailstones[i].position.x) - (hailstones[i].velocity.x * hailstones[i].position.y);

            long double aj = hailstones[j].velocity.y;
            long double bj = (-1 * hailstones[j].velocity.x);
            long double cj = (hailstones[j].velocity.y * hailstones[j].position.x) - (hailstones[j].velocity.x * hailstones[j].position.y);
            if (ai * bj == bi * aj)
            {
                continue;
            }
            long double x = ((ci * bj) - (cj * bi)) / ((ai * bj) - (aj * bi));
            long double y = ((cj * ai) - (ci * aj)) / ((ai * bj) - (aj * bi));

            bool firstInFuture  = ((x - hailstones[i].position.x) * hailstones[i].velocity.x >= 0 && (y - hailstones[i].position.y) * hailstones[i].velocity.y >= 0) ? true : false;
            bool secondInFuture = ((x - hailstones[j].position.x) * hailstones[j].velocity.x >= 0 && (y - hailstones[j].position.y) * hailstones[j].velocity.y >= 0) ? true : false;

            if (x >= min && x <= max && y >= min && y <= max && firstInFuture && secondInFuture)
            {
                sum++;
            }
        }
    }

    printf("sum: %lld\n", sum);

    return 0;
}
