#ifndef ROUTER_H
#define ROUTER_H

#include "trie.h"
#include "pq.h"

typedef struct router_stats {
    int routed;
    int dropped;
    int bytes_routed;
    int bytes_dropped;
    int packet_count_by_type[2]; // LOW and HIGH at the moment
    int eth_counts[4];           // 0 -> eth0, 1 -> eth1, until eth3...
} router_stats;

priority_queue* load_packets(char* file_name);
trie_node* load_routes(char* file_name);

router_stats process_packets(priority_queue* pq, trie_node* root);
void print_stats(router_stats stats);

#endif
