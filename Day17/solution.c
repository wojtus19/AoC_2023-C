#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXAMPLE (1)

#if EXAMPLE
#define INPUT_FILE "./example.txt"
#define GRID_SIZE 13
#else
#define INPUT_FILE "./input.txt"
#define GRID_SIZE 141
#endif // EXAMPLE

#define INIT_SIZE 262144
#define LINE_LEN 150

typedef struct HeapNode_t
{
    int heatLoss;
    int posY;
    int posX;
    int dx;
    int dy;
    int timesMoved;
} HeapNode_t;

typedef struct Heap_t
{
    HeapNode_t* data;
    int size;
    int items;
} Heap_t;

Heap_t InitHeap(int size)
{
    Heap_t v;
    v.size  = size;
    v.items = 0;
    v.data  = (HeapNode_t*)malloc(sizeof(HeapNode_t) * size);
    if (v.data != NULL)
    {
        memset(v.data, 0, sizeof(HeapNode_t) * size);
    }
    else
    {
        printf("Couldn't initialize heap.\n");
    }
    return v;
}

void Swap(HeapNode_t* a, HeapNode_t* b)
{
    HeapNode_t temp = *a;
    *a              = *b;
    *b              = temp;
}

void Heapify(Heap_t* heap)
{
    for (int i = (heap->items / 2) - 1; i >= 0; i--)
    {
        if (1 != heap->items)
        {
            int smallest = i;
            int l        = 2 * i + 1;
            int r        = 2 * i + 2;
            if (l < heap->items && heap->data[l].heatLoss < heap->data[smallest].heatLoss)
            {
                smallest = l;
            }
            if (r < heap->items && heap->data[r].heatLoss < heap->data[smallest].heatLoss)
            {
                smallest = r;
            }
            if (smallest != i)
            {
                Swap(&heap->data[i], &heap->data[smallest]);
                Heapify(heap);
            }
        }
    }
}

HeapNode_t HeapPop(Heap_t* heap)
{
    HeapNode_t popItem = heap->data[0];
    Swap(&heap->data[0], &heap->data[heap->items - 1]);
    heap->items -= 1;
    Heapify(heap);

    return popItem;
}

void InsertHeap(Heap_t* heap, HeapNode_t value)
{

    if (heap->items >= heap->size)
    {
        printf("\nReallocating.\n");
        heap->data = (HeapNode_t*)realloc(heap->data, sizeof(HeapNode_t) * heap->size * 2);
        if (heap->data == NULL)
        {
            printf("Failed memory allocation for heap\n");
            return;
        }
        memset(&heap->data[heap->size], 0, sizeof(HeapNode_t) * heap->size);
        heap->size = heap->size * 2;
    }
    heap->data[heap->items] = value;
    heap->items++;
}

void PrintHeap(Heap_t heap)
{
    for (int i = 0; i < heap.items; i++)
    {
        printf("node %d, heatloss: %d, y: %d, x: %d, dy: %d, dx: %d, timesMoved: %d\n", i, heap.data[i].heatLoss, heap.data[i].posY, heap.data[i].posX,
               heap.data[i].dx, heap.data[i].dy, heap.data[i].timesMoved);
    }
    printf("\n");
}

void FreeHeap(Heap_t* heap)
{
    free(heap->data);
}

bool InSeen(Heap_t seen, HeapNode_t hn)
{
    for (int i = 0; i < seen.items; i++)
    {
        if (seen.data[i].dx == hn.dx && seen.data[i].dy == hn.dy && seen.data[i].posX == hn.posX &&
            seen.data[i].posY == hn.posY && seen.data[i].timesMoved == hn.timesMoved)
            return true;
    }
    return false;
}

int Dijkstra(int grid[GRID_SIZE][GRID_SIZE])
{

    int heatLoss = 0;

    int directions[4][2] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };

    HeapNode_t data  = { .heatLoss = 0, .posX = 0, .posY = 0, .dx = 0, .dy = 0, .timesMoved = 0 };
    Heap_t priorityQ = InitHeap(INIT_SIZE);
    Heap_t seen      = InitHeap(INIT_SIZE);

    printf("Init done\n");

    InsertHeap(&priorityQ, data);
    while (priorityQ.items > 0)
    {
        Heapify(&priorityQ);
        HeapNode_t current = HeapPop(&priorityQ);

        if (current.posY == GRID_SIZE - 1 && current.posX == GRID_SIZE - 1 && current.timesMoved >= 4)
        {
            heatLoss = current.heatLoss;
            break;
        }

        if (current.posY < 0 || current.posY >= GRID_SIZE || current.posX < 0 || current.posX >= GRID_SIZE)
            continue;

        if (InSeen(seen, current))
            continue;

        InsertHeap(&seen, current);

        if (current.timesMoved < 10 && (current.dx != 0 || current.dy != 0))
        {

            int newX = current.posX + current.dx;
            int newY = current.posY + current.dy;
            if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE)
            {
                HeapNode_t newNode = { .heatLoss = current.heatLoss + grid[newY][newX], .posX = newX, .posY = newY, .dx = current.dx, .dy = current.dy, .timesMoved = current.timesMoved + 1 };
                InsertHeap(&priorityQ, newNode);
            }
        }
        if (current.timesMoved >= 4 || (current.dx == 0 && current.dy == 0))
        {
            for (int i = 0; i < 4; i++)
            {

                if ((directions[i][0] != current.dx || directions[i][1] != current.dy) && (directions[i][0] != (-1 * current.dx) || directions[i][1] != (-1 * current.dy)))
                {
                    int newDx = directions[i][0];
                    int newDy = directions[i][1];
                    int newX  = current.posX + newDx;
                    int newY  = current.posY + newDy;
                    if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE)
                    {
                        HeapNode_t newNode = { .heatLoss = current.heatLoss + grid[newY][newX], .posX = newX, .posY = newY, .dx = newDx, .dy = newDy, .timesMoved = 1 };
                        InsertHeap(&priorityQ, newNode);
                    }
                }
            }
        }
    }

    FreeHeap(&priorityQ);
    FreeHeap(&seen);
    return heatLoss;
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

    int grid[GRID_SIZE][GRID_SIZE];

    char* p;
    for (int i = 0; i < GRID_SIZE; i++)
    {
        p = fgets(line, sizeof(line), pInputFile);
        if (p == NULL)
            return -1;

        for (int j = 0; j < GRID_SIZE; j++)
        {
            grid[i][j] = line[j] - '0';
        }
    }

    int res = Dijkstra(grid);

    printf("sum: %d\n", res);
    return 0;
}