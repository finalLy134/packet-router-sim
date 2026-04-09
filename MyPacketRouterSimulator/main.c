#include "router.h"

#include <stdio.h>

int main() {
	trie_node* routes = load_routes("routes.txt");
	priority_queue* packets = load_packets("packets.txt");
	router_stats stats = process_packets(packets, routes);

	print_stats(stats);

	free_trie(routes);
	free_heap(packets);

	return 0;
}