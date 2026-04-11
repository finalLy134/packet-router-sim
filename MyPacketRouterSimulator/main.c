#include "router.h"

#include <stdio.h>

int main() {
	trie_node* routes = load_routes("routes.txt");
	priority_queue* packets = load_packets("packets.txt");
	hashtable* devices = load_devices("devices.txt");

	router_stats stats = process_packets(packets, routes, devices);

	print_stats(stats);

	free_trie(routes);
	free_heap(packets);

	return 0;
}