/**
 * File:        ll_basecode.h
 * Author:      Risto Heinsar
 * Created:     12.04.2016
 * Modified:    29.01.2026
 *
 * Description: Header for the linked list lab task basecode
 */
#ifndef LINKED_LIST_LAB_TASK
#define LINKED_LIST_LAB_TASK

typedef struct Node
{
    int id;
    char *name;
    struct Node *pNext;
} List;

enum MenuOpt {MENU_EXIT,
              MENU_PRINT,
              MENU_INSERT,
              MENU_FIND_ID,
              MENU_FIND_NAME,
              MENU_REM_ID,
              MENU_REM_NAME,
              MENU_CNT};

/* These 4 functions have already been completed for you */

void PrintList(List *pHead);

void PrintMenu(void);
int GetInputInRange(int min, int max);
void Printlist(List *pHead);
void ClearInputBuf(void);
void FreeList(struct Node *head);

/**
 * Functions to implement for the BASE TASK!
 */
void PrintNode(List *pnode);
struct Node *createNode(const char *name);
void insertNode(List **pHead, struct Node *pnode);
void unload(List *pHead);
struct Node *findNodeByName(List *pHead, char *name);
struct Node *findNodeByID(List *pHead, int id);

/**
 * Functions to implement for the EXTRA TASK 2
 */
int removeNodeByID(List **pHead, int id);
int removeNodeByName(List **pHead, const char *name);
void ReadLine(char *buffer, size_t size);
#endif // LINKED_LIST_LAB_TASK
