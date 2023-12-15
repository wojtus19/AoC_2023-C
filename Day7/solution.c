#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define LINE_LEN 10

#define EXAMPLE (0)
#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define HANDS_NUMBER 5
#else
#define INPUT_FILE "./input.txt"
#define HANDS_NUMBER 1000
#endif

#define CARDS_IN_HAND 5

typedef enum CardValue_t
{
    NONE  = 0,
    JOKER = 1,
    TWO   = 2,
    THREE = 3,
    FOUR  = 4,
    FIVE  = 5,
    SIX   = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE  = 9,
    TEN   = 10,
    QUEEN = 12,
    KING  = 13,
    ACE   = 14
} CardValue_t;

typedef enum HandType_t
{
    HIGH_CARD = 1,
    ONE_PAIR,
    TWO_PAIRS,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND,
} HandType_t;

typedef struct Hand
{
    CardValue_t cards[5];
    int bid;
    HandType_t type;
} Hand_t;

CardValue_t searchForHandType(CardValue_t const hand[CARDS_IN_HAND], int howMany, CardValue_t skip, int* pJokersLeft)
{
    int found          = 1;
    int jokersLeftCopy = *pJokersLeft;
    for (int i = 0; i < CARDS_IN_HAND; i++)
    {
        if (hand[i] == skip)
        {
            continue;
        }
        for (int j = 0; j < CARDS_IN_HAND; j++)
        {
            if (i == j || hand[j] == skip || (hand[j] == JOKER && jokersLeftCopy <= 0))
            {
                continue;
            }
            if (hand[j] == JOKER && jokersLeftCopy > 0)
            {
                found++;
                jokersLeftCopy--;
            }

            else if (hand[i] == hand[j])
            {
                found++;
            }
        }
        if (found == howMany)
        {
            (*pJokersLeft) = jokersLeftCopy;
            return hand[i];
        }
        jokersLeftCopy = *pJokersLeft;
        found          = 1;
    }
    (*pJokersLeft) = jokersLeftCopy;
    return NONE;
}

HandType_t getType(CardValue_t const hand[CARDS_IN_HAND])
{
    CardValue_t found = 0;
    int jokersLeft    = 0;
    for (int i = 0; i < CARDS_IN_HAND; i++)
    {
        if (hand[i] == 1)
        {
            jokersLeft++;
        }
    }
    found = searchForHandType(hand, 5, NONE, &jokersLeft); // search for Five of a kind

    if (found > NONE)
        return FIVE_OF_A_KIND;

    found = searchForHandType(hand, 4, NONE, &jokersLeft); // search for four of a kind

    if (found > NONE)
        return FOUR_OF_A_KIND;

    found = searchForHandType(hand, 3, NONE, &jokersLeft); // search for Three of a kind

    if (found > NONE)
    {
        found = searchForHandType(hand, 2, found, &jokersLeft); // Check if it's full hous
        if (found > NONE)
            return FULL_HOUSE;
        else
            return THREE_OF_A_KIND;
    }

    found = searchForHandType(hand, 2, NONE, &jokersLeft); // search for a pair

    if (found > NONE)
    {
        found = searchForHandType(hand, 2, found, &jokersLeft); // Check if is there another pair
        if (found > NONE)
            return TWO_PAIRS;
        else
            return ONE_PAIR;
    }

    return HIGH_CARD;
}

int cmpCards(Hand_t const first, Hand_t const second)
{
    if (first.type > second.type)
        return 1;
    if (second.type > first.type)
        return -1;

    for (int idx = 0; idx < CARDS_IN_HAND; idx++)
    {
        if (first.cards[idx] > second.cards[idx])
            return 1;
        if (second.cards[idx] > first.cards[idx])
            return -1;
    }
    return 0;
}

void sortCards(Hand_t hands[HANDS_NUMBER])
{
    for (int i = 0; i < HANDS_NUMBER; i++)
    {
        for (int j = 0; j < HANDS_NUMBER; j++)
        {
            if (cmpCards(hands[j], hands[j + 1]) > 0)
            {
                Hand_t tmp   = hands[j];
                hands[j]     = hands[j + 1];
                hands[j + 1] = tmp;
            }
        }
    }
}

void printHands(Hand_t hands[HANDS_NUMBER])
{
    for (int idx = 0; idx < HANDS_NUMBER; idx++)
    {
        printf("cards: ");
        for (int i = 0; i < CARDS_IN_HAND; i++)
        {
            printf("%d ", hands[idx].cards[i]);
        }
        printf("\tbid: %d  ", hands[idx].bid);
        printf("\ttype: %d\n", hands[idx].type);
    }
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

    Hand_t hands[HANDS_NUMBER];

    int idx = 0;
    while (fgets(line, sizeof(line), pInputFile) != 0)
    {
        for (int i = 0; i < CARDS_IN_HAND; i++)
        {
            if (line[i] >= '2' && line[i] <= '9')
            {
                hands[idx].cards[i] = (CardValue_t)(line[i] - '0');
            }
            else
            {
                switch (line[i])
                {
                case 'T':
                    hands[idx].cards[i] = TEN;
                    break;
                case 'J':
                    hands[idx].cards[i] = JOKER;
                    break;
                case 'Q':
                    hands[idx].cards[i] = QUEEN;
                    break;
                case 'K':
                    hands[idx].cards[i] = KING;
                    break;
                case 'A':
                    hands[idx].cards[i] = ACE;
                    break;
                default:
                    break;
                }
            }
        }
        sscanf(&line[6], "%d", &hands[idx].bid);
        hands[idx].type = getType(hands[idx].cards);
        idx++;
    }

    sortCards(hands);

    // printHands(hands);

    long int sum = 0;
    for (int i = 0; i < HANDS_NUMBER; i++)
    {
        sum += (i + 1) * hands[i].bid;
    }

    printf("sum: %ld\n", sum);

    return 0;
}