/**
 * File:        example_bin_search_tree.h
 * Author:      Risto Heinsar
 * Created:     21.04.2021
 * Last edit:   25.04.2025
 *
 * Description: Header for binary search tree example code
 */

#ifndef BIN_SEARCH_TREE_EXAMPLE
#define BIN_SEARCH_TREE_EXAMPLE

/* Specifies expected command line arguments */
#define ARGS_CNT              2
#define ARGS_EXEC       argv[0]
#define ARGS_FILE_NAME  argv[1]
#define MAX_NAME              32

typedef struct Node
{
    char combinedName[MAX_NAME];
    unsigned count;
    struct Node *pLeft;
    struct Node *pRight;
} Tree;

typedef struct TimePoints
{
    clock_t start;
    clock_t read;
    clock_t print;
    clock_t free;
} TimePoints;


/**
 * Description:    Reads the data from the file provided and stores everything
 *                 in a binary search tree.
 *
 * Parameters:     data - duoble pointer to a tree (node).
 *                 file - string with the name (path) of the file to read from
 *
 * Return:         none
 */
void ReadData(Tree **data, char *file);

/**
 * Description:    Inserts a new node into a binary search tree. It traverses
 *                 the tree until it finds a correct (and unused) leaf and
 *                 creates and assigns a node into that place.
 *
 * Parameters:     t - duoble pointer to a tree (node).
 *                 value - value to be stored into the tree.
 *
 * Return:         none
 */
void InsertToTree(Tree **t, char *name);


/**
 * Description:    Creates a new tree node and assigns the passed value to it.
 *                 Also initializes the child nodes to NULL.
 *
 * Parameters:     val - value for the node being created.
 *
 * Return:         pointer to the newly created node.
 *                 NULL pointer if memory allocation failed.
 */
Tree *CreateNode(char* name);


/**
 * Description:    Prints out the binary tree, uses a depth-first approach NLR
 *                 (root - left - right) for traversal.
 *
 * Parameters:     t - pointer to a node in the binary tree.
 *
 * Return:         void
 */
void PrintTree(Tree *t);

void FreeTree(Tree *t);

void PrintTime(TimePoints t);

#endif
