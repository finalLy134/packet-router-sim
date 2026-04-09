#include "router.h"

#include <stdio.h>

int main() {
	trie_node* routes = load_routes("routes.txt");
	priority_queue* packets = load_packets("packets.txt");

	process_packets(packets, routes);

	free_trie(routes);
	free_heap(packets);

	return 0;
}