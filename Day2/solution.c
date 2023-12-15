#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#else
#define INPUT_FILE "./input.txt"
#endif // EXAMPLE

#define LINE_LEN 110

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

    int i;
    int currNum;
    int game      = 0;
    long long sum = 0;
    int maxRed = 0, maxGreen = 0, maxBlue = 0;
    while (fgets(line, sizeof(line), pInputFile) != 0)
    {
        printf("%s", line);
        game++;
        i        = 7 + floor(log10(game) + 1);
        currNum  = 0;
        maxRed   = 0;
        maxBlue  = 0;
        maxGreen = 0;

        while (i < strlen(line))
        {
            if (line[i] <= '9' && line[i] >= '0')
            {
                currNum = line[i] - '0';
            }
            i++;
            if (line[i] != ' ')
            {
                currNum *= 10;
                currNum += line[i] - '0';
                i++;
            }
            i++;
            switch (line[i])
            {
            case 'r':
                maxRed = currNum >= maxRed ? currNum : maxRed;
                break;
            case 'g':
                maxGreen = currNum >= maxGreen ? currNum : maxGreen;
                break;
            case 'b':
                maxBlue = currNum >= maxBlue ? currNum : maxBlue;
                break;
            default:
                break;
            }
            i += 3;
        }

        sum += (maxBlue * maxGreen * maxRed);
    }

    printf("%lld\n", sum);

    return 0;
}