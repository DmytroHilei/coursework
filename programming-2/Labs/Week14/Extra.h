#ifndef TRIE_EXAMPLE
#define TRIE_EXAMPLE

#include <stdio.h>
#include <time.h>

#define MAX_NAME 64

/*
 * Alphabet:
 * A-Z = 26
 * a-z = 26
 * space = 1
 * minus = 1
 *
 * Total = 54
 */
#define ALPHA_LEN 54

#define ARGS_CNT              2
#define ARGS_EXEC       argv[0]
#define ARGS_FILE_NAME  argv[1]

typedef struct TrieNode
{
    char ch;
    unsigned count;
    struct TrieNode *chars[ALPHA_LEN];
} Trie;

typedef struct TimePoints
{
    clock_t start;
    clock_t read;
    clock_t print;
    clock_t free;
} TimePoints;

Trie *CreateNode(char ch);
int CharToIndex(char ch);
void InsertToTrie(Trie *root, char *name);
void ReadData(Trie *root, char *file);
void PrintTrie(Trie *root, char *buffer, int depth);
void FreeTrie(Trie *root);
void PrintTime(TimePoints t);

#endif