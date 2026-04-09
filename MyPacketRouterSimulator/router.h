#ifndef ROUTER_H
#define ROUTER_H

#include "trie.h"
#include "pq.h"

priority_queue* load_packets(char* file_name);
trie_node* load_routes(char* file_name);

void process_packets(priority_queue* pq, trie_node* root);
void print_stats();

#endif
