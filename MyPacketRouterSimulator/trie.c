#include "trie.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

trie_node* create_node() {
	trie_node* new_node = malloc(sizeof(trie_node));

	if (new_node == NULL) {
		printf("Out of memory.\n");
		return NULL;
	}

	new_node->children[0] = new_node->children[1] = NULL;
	new_node->eth = NULL;

	return new_node;
}

void insert(trie_node* root, uint32_t ip, int mask, char* eth) {
	trie_node* current = root;

	// we use mask because we care only about the prefix
	for (int i = 31; i >= 32 - mask; i--) {
		int bit = (ip >> i) & 1;

		if (current->children[bit] == NULL) {
			current->children[bit] = create_node();
		}

		current = current->children[bit];
	}

	current->eth = _strdup(eth);
}

char* search(trie_node* root, uint32_t ip) {
	trie_node* current = root;
	char* best_match = root->eth;

	for (int i = 31; i >= 0; i--) {
		int bit = (ip >> i) & 1;

		if (current->children[bit] == NULL)
			break;

		current = current->children[bit];

		if (current->eth != NULL)
			best_match = current->eth;
	}

	return best_match;
}

void free_trie(trie_node* root) {
	if (root == NULL) return;

	free_trie(root->children[0]);
	free_trie(root->children[1]);

	if (root->eth != NULL)
		free(root->eth);

	free(root);
}