#include <math.h>
#include <stdio.h>
#include <string.h>

#define INPUT_FILE "./input.txt"
#define LINE_LEN 37

#define RACES_NUM 1

int main()
{

    FILE* pInputFile;
    char timeLine[LINE_LEN + 2];
    char distanceLine[LINE_LEN + 2];

    pInputFile = fopen(INPUT_FILE, "r");
    if (NULL == pInputFile)
    {
        printf("file cannot be opened\n");
        return -1;
    }

    long long bCoeff[RACES_NUM] = { 0 };
    long long cCoeff[RACES_NUM] = { 0 };

    if (fgets(timeLine, sizeof(timeLine), pInputFile) == 0)
        return -1;

    if (fgets(distanceLine, sizeof(distanceLine), pInputFile) == 0)
        return -1;

    int idx = 0;
    for (int i = 0; i < RACES_NUM; i++)
    {
        idx = 11 + 7 * i;
        sscanf(&timeLine[idx], "%lld", &bCoeff[i]);
        sscanf(&distanceLine[idx], "%lld", &cCoeff[i]);
    }

    // -x^2 + time*x - distance >= 0

    long double x1;
    long double x2;
    int sum = 1;
    for (int i = 0; i < RACES_NUM; i++)
    {

        x1 = (bCoeff[i] - sqrt(bCoeff[i] * bCoeff[i] - 4 * cCoeff[i])) / 2;
        x2 = (bCoeff[i] + sqrt(bCoeff[i] * bCoeff[i] - 4 * cCoeff[i])) / 2;

        x1 = ceil(x1) == x1 ? x1 + 1 : ceil(x1);
        x2 = floor(x2) == x2 ? x2 - 1 : floor(x2);

        // printf("x1: %Lf, x2: %Lf, diff: %d\n", x1, x2, (abs(x1 - x2 - 1)));

        sum *= (abs(x1 - x2 - 1));

        // printf("b: %lld\n", bCoeff[i]);
        // printf("c: %lld\n", cCoeff[i]);
    }

    printf("sum: %d\n", sum);

    return 0;
}