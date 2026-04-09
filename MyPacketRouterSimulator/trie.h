#ifndef TRIE_H
#define TRIE_H

#include <stdint.h>

typedef struct trie_node {
    struct trie_node* children[2]; // 0 or 1 bit
    char* eth;                     // etherent string at end of path, if eth != NULL then end
} trie_node;

trie_node* create_node();

void insert(trie_node* root, uint32_t ip, int mask, char* eth);
char* search(trie_node* root, uint32_t ip);

void free_trie(trie_node* root);

#endif
