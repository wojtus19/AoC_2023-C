#include <stdio.h>
#include <string.h>

#define EXAMPLE (0)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define PILE_SIZE 6
#define WINNINGS_CARDS_START 9
#define PLAYER_CARDS_START 24
#else
#define INPUT_FILE "./input.txt"
#define PILE_SIZE 218
#define WINNINGS_CARDS_START 9
#define PLAYER_CARDS_START 41
#endif

#define LINE_LEN 117

#define WINNING_CARDS_AMOUNT 10
#define PLAYER_CARDS_AMOUNT 25

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

    int winningCards[PILE_SIZE][WINNING_CARDS_AMOUNT] = { 0 };
    int playerCards[PILE_SIZE][PLAYER_CARDS_AMOUNT]   = { 0 };

    int i = 0;
    while (fgets(line, sizeof(line), pInputFile) != 0)
    {

        for (int j = 0; j < WINNING_CARDS_AMOUNT; j++)
        {
            int idx = (3 * j) + WINNINGS_CARDS_START;
            sscanf(&line[idx], "%d", &winningCards[i][j]);
        }

        for (int j = 0; j < PLAYER_CARDS_AMOUNT; j++)
        {
            int idx = (3 * j) + PLAYER_CARDS_START;
            sscanf(&line[idx], "%d", &playerCards[i][j]);
        }
        i++;
    }

    int instances[PILE_SIZE] = { [0 ... PILE_SIZE - 1] = 1 };

    for (int j = 0; j < PILE_SIZE; j++)
    {
        for (int x = 0; x < instances[j]; x++)
        {
            int points = 0;
            for (int k = 0; k < PLAYER_CARDS_AMOUNT; k++)
            {
                for (int l = 0; l < WINNING_CARDS_AMOUNT; l++)
                {
                    if (winningCards[j][l] == playerCards[j][k])
                    {
                        points++;
                    }
                }
            }

            for (int n = 1; n <= points; n++)
            {
                if (j + n < PILE_SIZE)
                {
                    instances[j + n]++;
                }
            }
        }
    }

    int sum = 0;
    for (int j = 0; j < PILE_SIZE; j++)
    {
        // printf("j: %d, instances %d\n", j, instances[j]);
        sum += instances[j];
    }

    printf("sum: %d\n", sum);

    return 0;
}