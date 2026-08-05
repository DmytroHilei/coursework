/**
 * File:        example_bin_search_tree.c
 * Author:      Risto Heinsar
 * Created:     21.04.2021
 * Last edit:   25.04.2025
 *
 * Description: Sample code to demonstrate creation and traversal of binary
 *              search trees. Provided code works with integers read from a file.
 *
 * Usage:       ./program_name name_of_input_file
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "example_bin_search_tree.h"

FILE* gOutputFile = NULL;

unsigned gNameCount = 0;

int main(int argc, char *argv[])
{
    /* Makes sure that the input file name was passed to the program */
    if (argc != ARGS_CNT)
    {
        fprintf(stderr, "Usage: %s data_file!\n", ARGS_EXEC);
        exit(EXIT_FAILURE);
    }

    Tree *data = NULL;
    TimePoints timePoints;

    timePoints.start = clock();

    ReadData(&data, ARGS_FILE_NAME);

    timePoints.read = clock();

    gOutputFile = fopen("tree_out.txt", "w");

    if (gOutputFile == NULL) {
        perror("Opening of output file failed");
        FreeTree(data);
        exit(EXIT_FAILURE);
    }

    PrintTree(data);
    printf("\n");

    fclose(gOutputFile);
    gOutputFile = NULL;

    timePoints.print = clock();

    FreeTree(data);

    timePoints.free = clock();

    printf("Found %u names\n", gNameCount);
    printf("\n");
    PrintTime(timePoints);

    return EXIT_SUCCESS;
}

/**
 * Reads through a file with integers, stores them into a binary search tree
 */

void ReadData(Tree **data, char *file)
{
    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        perror("Opening of input file failed");
        exit(EXIT_FAILURE);
    }

    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    char combinedName[MAX_NAME];

    while (fscanf(fp, "%*s %31s %31s %*s", firstName, lastName) == 2)
    {
        snprintf(combinedName, sizeof(combinedName), "%s %s", firstName, lastName);
        InsertToTree(data, combinedName);
    }

    fclose(fp);
}


/**
 * Inserts node into the binary search tree, ignores duplicate values
 */
void InsertToTree(Tree **t, char *name)
{
    Tree *currentNode = *t;

    if (currentNode == NULL)
    {
        *t = CreateNode(name);
        gNameCount++;
        return;
    }

    int cmp = strcmp(name, currentNode->combinedName);

    if (cmp < 0)
    {
        InsertToTree(&(currentNode->pLeft), name);
    }
    else if (cmp > 0)
    {
        InsertToTree(&(currentNode->pRight), name);
    }
    else
    {
        currentNode->count++;
    }
}


/**
 * Allocates and initializes a tree node, returns the node
 */

Tree *CreateNode(char *name)
{
    Tree *pNewNode = malloc(sizeof(Tree));
    if (pNewNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    strcpy(pNewNode->combinedName, name);
    pNewNode->count = 1;

    pNewNode->pLeft = NULL;
    pNewNode->pRight = NULL;

    return pNewNode;
}

/**
 * Traverses recursively through the tree in pre-order and prints all nodes
 */

void PrintTree(Tree *t)
{
    if (t != NULL)
    {
        PrintTree(t->pLeft);

        fprintf(gOutputFile, "%s %u\n", t->combinedName, t->count);

        PrintTree(t->pRight);
    }
}


void FreeTree(Tree *t) {
    if (t != NULL) {
        FreeTree(t->pLeft);
        FreeTree(t->pRight);
        free(t);
    }
}


void PrintTime(TimePoints t)
{
    printf("Reading:  %9.6lf s\n", (double)(t.read - t.start) / CLOCKS_PER_SEC);
    printf("Printing: %9.6lf s\n", (double)(t.print - t.read) / CLOCKS_PER_SEC);
    printf("Freeing:  %9.6lf s\n", (double)(t.free - t.print) / CLOCKS_PER_SEC);
    printf("Total:    %9.6lf s\n", (double)(t.free - t.start) / CLOCKS_PER_SEC);
}


