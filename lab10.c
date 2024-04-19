#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26  // Alphabet size for lowercase English letters

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int count;  // This counts the number of times a word is inserted
};

// Trie structure
struct Trie {
    struct TrieNode *root;
};

// Helper function to create a new trie node
struct TrieNode *createNode(void) {
    struct TrieNode *pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    pNode->count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pNode->children[i] = NULL;
    }
    return pNode;
}

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    pTrie->root = createNode();
    return pTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
    struct TrieNode *pCrawl = pTrie->root;
    while (*word) {
        int index = *word - 'a';
        if (!pCrawl->children[index]) {
            pCrawl->children[index] = createNode();
        }
        pCrawl = pCrawl->children[index];
        word++;
    }
    pCrawl->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word) {
    struct TrieNode *pCrawl = pTrie->root;
    while (*word) {
        int index = *word - 'a';
        if (!pCrawl->children[index]) {
            return 0;
        }
        pCrawl = pCrawl->children[index];
        word++;
    }
    return pCrawl->count;
}

// Helper function to deallocate a trie node recursively
void deallocateNode(struct TrieNode *pNode) {
    if (pNode == NULL) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        deallocateNode(pNode->children[i]);
    }
    free(pNode);
}

// Deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie) {
    if (pTrie == NULL) return NULL;
    deallocateNode(pTrie->root);
    free(pTrie);
    return NULL;
}

// Function to read the dictionary file and allocate words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    int numWords;
    fscanf(file, "%d", &numWords);  // Read the number of words

    char wordBuffer[100];
    for (int i = 0; i < numWords; i++) {
        fscanf(file, "%s", wordBuffer);
        pInWords[i] = strdup(wordBuffer);
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];
    
    int numWords = readDictionary("dictionary.txt", inWords);
    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");

    // Free the word list
    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }

    return 0;
}
