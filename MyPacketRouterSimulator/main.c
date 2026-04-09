#include "router.h"

#include <stdio.h>

int main() {
	trie_node* routes = load_routes("routes.txt");
	priority_queue* packets = load_packets("packets.txt");

	process_packets(packets, routes);

	return 0;
}