#include <stdio.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#else
#define INPUT_FILE "./input.txt"
#endif // EXAMPLE

#define LINE_LEN 55

#define DIGIT_NUM 9

char* digitsAsString[DIGIT_NUM] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

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

    int firstNum  = 0;
    int secondNum = 0;
    int sum       = 0;

    while (fgets(line, sizeof(line), pInputFile) != 0)
    {
        firstNum    = 0;
        secondNum   = 0;
        int lineLen = strlen(line);
        // search for first
        for (int idx = 0; idx < lineLen; idx++)
        {
            if (line[idx] >= '0' && line[idx] <= '9')
            {
                firstNum = line[idx] - '0';
                break;
            }
            else
            {
                for (int i = 0; i < DIGIT_NUM; i++)
                {
                    if (strncmp(digitsAsString[i], &line[idx], strlen(digitsAsString[i])) == 0)
                    {
                        firstNum = i + 1;
                        goto found_first;
                    }
                }
            }
        }
    found_first:

        // search for second
        for (int idx = lineLen - 1; idx >= 0; idx--)
        {
            if (line[idx] >= '0' && line[idx] <= '9')
            {
                secondNum = line[idx] - '0';
                break;
            }
            else
            {
                for (int i = 0; i < DIGIT_NUM; i++)
                {
                    int digitLen = strlen(digitsAsString[i]);
                    if (idx - digitLen >= 0)
                    {
                        if (strncmp(digitsAsString[i], &line[idx - digitLen], digitLen) == 0)
                        {
                            secondNum = i + 1;
                            goto found_second;
                        }
                    }
                }
            }
        }
    found_second:

        printf("firstNum: %d, secondNum: %d\n\n", firstNum, secondNum);
        sum += (firstNum * 10) + secondNum;
    }

    printf("sum %d\n", sum);

    return 0;
}