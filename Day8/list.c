#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

void Push(List_t **top, char data[DATA_SIZE])
{
    List_t *newNode;
    newNode = (List_t *)malloc(sizeof(List_t));
    strcpy(newNode->data, data);
    newNode->left = NULL;
    newNode->right = NULL;

    if (NULL == top)
    {
        newNode->next = NULL;
    }
    else
    {
        newNode->next = *top;
    }

    *top = newNode;
}

void AddLeft(List_t *list, char whereToAddData[DATA_SIZE], char data[DATA_SIZE])
{
    List_t *whereToAdd = Find(list, whereToAddData);
    List_t *nodeToAdd = Find(list, data);
    if (nodeToAdd != NULL)
    {
        whereToAdd->left = nodeToAdd;
    }
}

void AddRight(List_t *list, char whereToAddData[DATA_SIZE], char data[DATA_SIZE])
{
    List_t *whereToAdd = Find(list, whereToAddData);
    List_t *nodeToAdd = Find(list, data);
    if (nodeToAdd != NULL)
    {
        whereToAdd->right = nodeToAdd;
    }
}

char *Pop(List_t **top)
{
    if (NULL != *top)
    {
        List_t *temp = *top;
        char *tempData = malloc(sizeof(char) * DATA_SIZE);
        strcpy(tempData, (*top)->data);
        *top = (*top)->next;
        free(temp);
        return tempData;
    }
    return 0;
}

List_t *Find(List_t *top, char data[DATA_SIZE])
{
    if (NULL == top)
    {
        return NULL;
    }
    List_t *tmp = top;
    while (tmp != NULL && strcmp(tmp->data, data) != 0)
    {
        tmp = tmp->next;
    }
    if (tmp != NULL)
    {
        return tmp;
    }
    else
    {
        printf("Not found!\n");
        return NULL;
    }
}

void PrintList(List_t *top)
{
    if (NULL == top)
    {
        printf("List is empty.\n");
    }
    else
    {
        List_t *temp = top;
        while (NULL != temp->next)
        {
            printf("Node: %s, Left: %s, Right: %s\n", temp->data, temp->left->data, temp->right->data);
            temp = temp->next;
        }
        printf("Node: %s, Left: %s, Right: %s\nNULL\n\n", temp->data, temp->left->data, temp->right->data);
    }
}
