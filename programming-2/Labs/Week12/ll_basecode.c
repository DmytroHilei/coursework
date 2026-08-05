/**
 * File:        ll_basecode.c
 * Author:      Risto Heinsar
 * Created:     12.04.2016
 * Modified:    29.01.2026
 *
 * Description: Linked list lab task basecode.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ll_basecode.h"

#define MAX_ID 1000000
#define MIN_ID 1

int main(void)
{
    List *pHead = NULL;


    /**************************************************************************
     *  TESTING STAGE 1: PrintNode is complete                                *
     **************************************************************************/

    // PrintNode(&(struct Node){.id = 2, .name = "Maamees Jaanes", .pNext = NULL});
    // PrintNode(NULL);

    // return EXIT_SUCCESS;


    /**************************************************************************
     *  TESTING STAGE 2: PrintList is complete                                *
     **************************************************************************/
    /* struct Node c = {.id = 2, .name = "Maamees Jaanes", .pNext = NULL};
    struct Node b = {.id = 1, .name = "Linnud Tõnisted", .pNext = &c};
    struct Node a = {.id = 0, .name = "Kivipallur Jürto Nii-Dommzonn", .pNext = &b};

    pHead = &a;

    PrintList(pHead);

    return EXIT_SUCCESS; */


    /**************************************************************************
     *  TESTING STAGE 3: CreateNode() is complete                             *
     **************************************************************************/
    /* struct Node *a = createNode("Kivipallur Jürto Nii-Dommzonn");
    struct Node *b = createNode("Linnud Tõnisted");
    struct Node *c = createNode("Maamees Jaanes");

    b->pNext = c;
    a->pNext = b;
    pHead = a;

    PrintList(pHead);

    FreeList(pHead);
    pHead = NULL;

    return EXIT_SUCCESS; */


    /**************************************************************************
     *  TESTING STAGE 4: InsertNode() is complete                             *
     **************************************************************************/
    /* struct Node *a = createNode("Kivipallur Jürto Nii-Dommzonn");
    struct Node *b = createNode("Linnud Tõnisted");
    struct Node *c = createNode("Maamees Jaanes");

    insertNode(&pHead, c);
    insertNode(&pHead, b);
    insertNode(&pHead, a);

    PrintList(pHead);

    FreeList(pHead);
    pHead = NULL;

    return EXIT_SUCCESS; */


    /**************************************************************************
     *  TESTING STAGE 5: Unload is completed                                  *
     **************************************************************************/
    /* struct Node *a = createNode("Kivipallur Jürto Nii-Dommzonn");
    struct Node *b = createNode("Linnud Tõnisted");
    struct Node *c = createNode("Maamees Jaanes");

    insertNode(&pHead, c);
    insertNode(&pHead, b);
    insertNode(&pHead, a);

    unload(pHead);

    return EXIT_SUCCESS; */


    /**************************************************************************
     *  TESTING STAGE 6: Both Find() functions are complete                   *
     **************************************************************************/
    /* struct Node *a = createNode("Kivipallur Jürto Nii-Dommzonn");
    struct Node *b = createNode("Linnud Tõnisted");
    struct Node *c = createNode("Maamees Jaanes");

    insertNode(&pHead, c);
    insertNode(&pHead, b);
    insertNode(&pHead, a);

    PrintNode(findNodeByName(pHead, "Linnud Tõnisted"));
    PrintNode(findNodeByName(pHead, "meow"));

    PrintNode(findNodeByID(pHead, 2));
    PrintNode(findNodeByID(pHead, 5));

    unload(pHead);
    return EXIT_SUCCESS; */


    while (1)
{
    PrintMenu();

    switch (GetInputInRange(0, MENU_CNT - 1))
    {
        case MENU_EXIT:
            unload(pHead);
            pHead = NULL;
            exit(EXIT_SUCCESS);
            break;

        case MENU_PRINT:
            PrintList(pHead);
            break;

        case MENU_INSERT:
        {
            char name[128];

            printf("Enter name: ");
            ClearInputBuf();
            ReadLine(name, sizeof(name));

            if (name[0] == '\0') {
                printf("Name cannot be empty!\n");
                break;
            }

            List *newNode = createNode(name);

            if (newNode == NULL) {
                fprintf(stderr, "Failed to create node!\n");
                break;
            }

            insertNode(&pHead, newNode);
            printf("Node inserted.\n");

            break;
        }

        case MENU_FIND_ID:
        {
            int id;

            printf("Enter ID: ");
            id = GetInputInRange(MIN_ID, MAX_ID);

            List *found = findNodeByID(pHead, id);
            PrintNode(found);

            break;
        }

        case MENU_FIND_NAME:
        {
            char name[128];

            printf("Enter name: ");
            ClearInputBuf();
            ReadLine(name, sizeof(name));

            List *found = findNodeByName(pHead, name);
            PrintNode(found);

            break;
        }

        case MENU_REM_ID:
        {
            int id;

            printf("Enter ID to remove: ");
            id = GetInputInRange(MIN_ID, MAX_ID);

            if (removeNodeByID(&pHead, id)) {
                printf("Node removed.\n");
            } else {
                printf("Node not found.\n");
            }

            break;
        }

        case MENU_REM_NAME:
        {
            char name[128];

            printf("Enter name to remove: ");
            ClearInputBuf();
            ReadLine(name, sizeof(name));

            if (removeNodeByName(&pHead, name)) {
                printf("Node removed.\n");
            } else {
                printf("Node not found.\n");
            }

            break;
        }

        default:
            puts("Unrecognized input!");
            break;
    }
}
    return EXIT_SUCCESS;
}


/**
 * Returns an input between min and max, inclusive
 */
int GetInputInRange(int min, int max)
{
    int input;

    while (1)
    {
        printf("> ");
        scanf("%d", &input);

        if (input <= max && input >= min)
            return input;

        fprintf(stderr, "Input out of bounds [%d:%d]]!\n", min, max);
        ClearInputBuf();
    }
}

/**
 * Clears the input buffer
 */
void ClearInputBuf(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}


/**
 * Prints the available menu options
 */
void PrintMenu(void)
{
    puts("");
    puts("Select a task");

    printf("%d - Print list\n",           MENU_PRINT);
    printf("%d - Insert a node\n",        MENU_INSERT);
    printf("%d - Find a node by ID\n",    MENU_FIND_ID);
    printf("%d - Find a node by name\n",  MENU_FIND_NAME);
    printf("%d - Erase a node by ID\n",   MENU_REM_ID);
    printf("%d - Erase a node by name\n", MENU_REM_NAME);
    printf("%d - Exit\n",                 MENU_EXIT);
}


/**
 * Prints out the entire linked list
 */
void PrintList(List *pHead)
{
    if (pHead == NULL)
    {
        puts("List empty!");
        return;
    }

    for (List *pTemp = pHead; pTemp; pTemp = pTemp->pNext)
        PrintNode(pTemp);
}

void PrintNode (List *pnode)
{

    if (pnode == NULL) {
        printf("Error, node did not pass! \n \n");
        return;
    }

    printf("ID: %d\n", pnode->id);
    printf("Name: %s\n", pnode->name);
    printf("pNext: %p\n\n", (void *)pnode->pNext);
}

struct Node *createNode(const char *name)
{
    static int nextId = 1;

    struct Node *node = malloc(sizeof(struct Node));

    if (node == NULL) {
        return NULL;
    }

    node->name = strdup(name);

    if (node->name == NULL) {
        free(node);
        return NULL;
    }

    node->id = nextId++;
    node->pNext = NULL;

    return node;
}

void insertNode(List **pHead, struct Node *pnode)
{
    if (pHead == NULL || pnode == NULL) {
        return;
    }

    pnode->pNext = NULL;

    /* Case 1: empty list OR new node should become the first node */
    if (*pHead == NULL || strcmp(pnode->name, (*pHead)->name) < 0) {
        pnode->pNext = *pHead;
        *pHead = pnode;
        return;
    }

    /* Case 2: find position before the first node whose name is greater */
    List *pTemp = *pHead;

    while (pTemp->pNext != NULL &&
           strcmp(pTemp->pNext->name, pnode->name) <= 0) {
        pTemp = pTemp->pNext;
           }

    /* Insert after pTemp */
    pnode->pNext = pTemp->pNext;
    pTemp->pNext = pnode;
}


struct Node *findNodeByName(List *pHead, char *name)
{
    if (name == NULL) {
        return NULL;
    }

    for (List *pTemp = pHead; pTemp != NULL; pTemp = pTemp->pNext) {
        int cmp = strcmp(pTemp->name, name);

        if (cmp == 0) {
            return pTemp;
        }

        if (cmp > 0) {
            return NULL;
        }
    }

    return NULL;
}

struct Node *findNodeByID(List *pHead, int id) {
    if (pHead == NULL) {
        return NULL;
    }
    for (List *pTemp = pHead; pTemp; pTemp = pTemp->pNext) {
        if (pTemp->id == id) {
            return pTemp;
        }
    }
    return NULL;
}

void unload(List *pHead)
{
    List *pTemp = pHead;

    while (pTemp != NULL) {
        List *pNext = pTemp->pNext;

        PrintNode(pTemp);
        free(pTemp);

        pTemp = pNext;
    }
}

int removeNodeByID(List **pHead, int id) {
    if (*pHead == NULL) {
        return 0;
    }
    if ((*pHead)->id == id) {
        List *pTemp = *pHead;
        *pHead = (*pHead)->pNext;
        free(pTemp);
        return 1;
    }
    return 0;
}

int removeNodeByName(List **pHead, const char *name)
{
    if (pHead == NULL || *pHead == NULL || name == NULL) {
        return 0;
    }

    List *current = *pHead;
    List *previous = NULL;

    while (current != NULL) {
        int cmp = strcmp(current->name, name);

        if (cmp == 0) {
            if (previous == NULL) {
                *pHead = current->pNext;
            } else {
                previous->pNext = current->pNext;
            }

            free(current->name);  /* only if you used strdup */
            free(current);

            return 1;
        }

        if (cmp > 0) {
            return 0;
        }

        previous = current;
        current = current->pNext;
    }

    return 0;
}

void FreeList(struct Node *head)
{
    struct Node *current = head;

    while (current != NULL) {
        struct Node *next = current->pNext;
        free(current);
        current = next;
    }
}

void ReadLine(char *buffer, size_t size)
{
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}