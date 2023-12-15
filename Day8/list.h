#define DATA_SIZE 3

typedef struct List_t
{
    char data[DATA_SIZE];
    struct List_t *next;
    struct List_t *left;
    struct List_t *right;
} List_t;

// void Push(List_t **top, char data[DATA_SIZE]);
void Push(List_t **top, char data[DATA_SIZE]);

void AddLeft(List_t* list, char nodeDada[DATA_SIZE], char data[DATA_SIZE]);

void AddRight(List_t* list, char nodeDada[DATA_SIZE], char data[DATA_SIZE]);

char *Pop(List_t **top);

List_t *Find(List_t *top, char data[DATA_SIZE]);

void PrintList(List_t *top);
