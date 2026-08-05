#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Extra.h"

/* Global output file pointer, same optimization idea as BST */
FILE *gOutputFile = NULL;

/* Number of different name combinations */
unsigned gNameCount = 0;

int main(int argc, char *argv[]) {

    if (argc != ARGS_CNT)
    {
        fprintf(stderr, "Usage: %s data_file!\n", ARGS_EXEC);
        exit(EXIT_FAILURE);
    }

    Trie *root = CreateNode('\0');
    TimePoints timePoints;

    timePoints.start = clock();

    ReadData(root, ARGS_FILE_NAME);

    timePoints.read = clock();

    gOutputFile = fopen("trie_output.txt", "w");
    if (gOutputFile == NULL)
    {
        perror("Opening of output file failed");
        FreeTrie(root);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_NAME];
    PrintTrie(root, buffer, 0);

    fclose(gOutputFile);
    gOutputFile = NULL;

    timePoints.print = clock();

    FreeTrie(root);

    timePoints.free = clock();

    printf("Found %u names\n", gNameCount);
    printf("\n");
    PrintTime(timePoints);

    return EXIT_SUCCESS;
}


Trie *CreateNode(char ch)
{
    Trie *node = malloc(sizeof(Trie));
    if (node == NULL)
    {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    node->ch = ch;
    node->count = 0;

    for (int i = 0; i < ALPHA_LEN; i++)
    {
        node->chars[i] = NULL;
    }

    return node;
}

int CharToIndex(char ch)
{
    if (ch >= 'A' && ch <= 'Z')
    {
        return ch - 'A';
    }

    if (ch >= 'a' && ch <= 'z')
    {
        return 26 + (ch - 'a');
    }

    if (ch == ' ')
    {
        return 52;
    }

    if (ch == '-')
    {
        return 53;
    }

    return -1;
}

void InsertToTrie(Trie *root, char *name)
{
    Trie *current = root;

    for (int i = 0; name[i] != '\0'; i++)
    {
        int idx = CharToIndex(name[i]);

        if (idx == -1)
        {
            fprintf(stderr, "Unsupported character '%c' in name: %s\n", name[i], name);
            return;
        }

        if (current->chars[idx] == NULL)
        {
            current->chars[idx] = CreateNode(name[i]);
        }

        current = current->chars[idx];
    }

    if (current->count == 0)
    {
        gNameCount++;
    }

    current->count++;
}


void ReadData(Trie *root, char *file)
{
    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        perror("Opening of input file failed");
        exit(EXIT_FAILURE);
    }

    char firstName[32];
    char lastName[32];
    char combinedName[MAX_NAME];

    while (fscanf(fp, "%*s %31s %31s %*s", firstName, lastName) == 2)
    {
        snprintf(combinedName, sizeof(combinedName), "%s %s", firstName, lastName);
        InsertToTrie(root, combinedName);
    }

    fclose(fp);
}



void PrintTrie(Trie *root, char *buffer, int depth)
{
    if (root == NULL)
    {
        return;
    }

    if (root->count > 0)
    {
        buffer[depth] = '\0';
        fprintf(gOutputFile, "%s %u\n", buffer, root->count);
    }

    for (int i = 0; i < ALPHA_LEN; i++)
    {
        if (root->chars[i] != NULL)
        {
            buffer[depth] = root->chars[i]->ch;
            PrintTrie(root->chars[i], buffer, depth + 1);
        }
    }
}


void FreeTrie(Trie *root)
{
    if (root == NULL)
    {
        return;
    }

    for (int i = 0; i < ALPHA_LEN; i++)
    {
        FreeTrie(root->chars[i]);
    }

    free(root);
}



void PrintTime(TimePoints t)
{
    printf("Reading:  %9.6lf s\n", (double)(t.read - t.start) / CLOCKS_PER_SEC);
    printf("Printing: %9.6lf s\n", (double)(t.print - t.read) / CLOCKS_PER_SEC);
    printf("Freeing:  %9.6lf s\n", (double)(t.free - t.print) / CLOCKS_PER_SEC);
    printf("Total:    %9.6lf s\n", (double)(t.free - t.start) / CLOCKS_PER_SEC);
}