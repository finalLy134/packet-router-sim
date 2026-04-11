#ifndef ROUTER_H
#define ROUTER_H

#include "trie.h"
#include "pq.h"
#include "hashtable.h"

typedef struct router_stats {
    int routed;
    int dropped;
    int no_mac;
    int delivered;
    int bytes_routed;
    int bytes_dropped;
    int bytes_delivered;
    int packet_count_by_type[2]; // LOW and HIGH
    int eth_counts[4];           // 0 -> eth0, 1 -> eth1, until eth3...
} router_stats;

typedef struct packet_result {
    int has_route;
    int has_mac;
} packet_result;

priority_queue* load_packets(char* file_name);
trie_node* load_routes(char* file_name);
hashtable* load_devices(char* file_name);

packet_result resolve_packet(packet* cur, trie_node* root, hashtable* devices, char* eth_out, char* mac_out);
void print_packet(int count, packet* cur, const char* eth, const char* mac, packet_result res);
void update_stats(router_stats* stats, packet* cur, const char* eth, packet_result res);
router_stats process_packets(priority_queue* pq, trie_node* root, hashtable* devices);

void print_stats(router_stats stats);

#endif
