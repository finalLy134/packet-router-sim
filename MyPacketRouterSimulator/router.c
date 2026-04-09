#include "router.h"
#include "trie.h"
#include "packet.h"

#include <stdio.h>
#include <string.h>

priority_queue* load_packets(char* file_name) {
    FILE* fp = fopen(file_name, "r");

    if (fp == NULL) {
        printf("Could not open file %s for packet loading.", file_name);
        return NULL;
    }

    char buff[128];
    int count = 0;

    while (fgets(buff, sizeof(buff), fp))
        count++;

    priority_queue* pq = create_pq(count);

    if (pq == NULL) {
        printf("Out of memory.\n");
        fclose(fp);
        return NULL;
    }

    rewind(fp);
    for (int i = 0; i < count; i++) {
        fgets(buff, sizeof(buff), fp);

        char src[32], dest[32], priority[32];
        int size;

        sscanf_s(buff, "%s %s %s %d", src, 32, dest, 32, priority, 32, &size);

        packet p = {
            parse_ip(src),
            parse_ip(dest),
            (strcmp(priority, "HIGH") == 0) ? HIGH : LOW,
            size
        };

        enqueue(pq, p);
    }

    fclose(fp);
    return pq;
}

trie_node* load_routes(char* file_name) {
    FILE* fp = fopen(file_name, "r");

    if (fp == NULL) {
        printf("Could not open file %s for routing.", file_name);
        return;
    }

    trie_node* root = create_node();

    if (root == NULL) {
        printf("Out of memory.\n");
        fclose(fp);
        return NULL;
    }

    char buff[128];
    int count = 1;

    while (fgets(buff, sizeof(buff), fp)) {
        char full_ip[64], eth[32];
        int subnet_mask = 0;

        sscanf_s(buff, "%s %s", full_ip, 64, eth, 32);

        char ip[64];
        // we use a scanset pattern that will firstly give us the whole ip until the '/'
        // (%[^/] means take everything but the '/')
        // afterwards we skip the '/' and take the subset mask
        sscanf_s(full_ip, "%[^/]/%d", ip, 64, &subnet_mask);

        insert(root, parse_ip(ip), subnet_mask, eth);
    }

    fclose(fp);
    return root;
}

router_stats process_packets(priority_queue* pq, trie_node* root) {
    router_stats stats = { 0 };
    int count = 1;

    while (!heap_is_empty(pq)) {
        packet cur = dequeue(pq);
        char src[32], dest[32];
        stringify_ip(cur.src, src, sizeof(src));
        stringify_ip(cur.dest, dest, sizeof(dest));

        char* eth = search(root, cur.dest);
        char* eth_str = eth != NULL ? eth : "----";
        char* status = eth != NULL ? "ROUTED" : "DROPPED";
        char* priority = cur.priority == HIGH ? "HIGH" : "LOW";

        printf("[PACKET #%03d] %-16s -> %-16s -> %-6s | Priority: %-4s | Size: %4d | %s\n",
            count, src, dest, eth_str, priority, cur.size, status);

        if (eth != NULL) {
            stats.routed++;
            stats.bytes_routed += cur.size;
            int eth_index = eth[3] - '0';
            stats.eth_counts[eth_index]++;
        }
        else {
            stats.dropped++;
            stats.bytes_dropped += cur.size;
        }

        stats.packet_count_by_type[cur.priority]++;
        count++;
    }
    return stats;
}

void print_stats(router_stats stats) {
    int total = stats.routed + stats.dropped;
    int busiest_eth = 0;
    for (int i = 1; i < 4; i++)
        if (stats.eth_counts[i] > stats.eth_counts[busiest_eth])
            busiest_eth = i;

    printf("\n--- Routing Stats -----------------------------------------------\n");
    printf("  Packets   : %d total, %d routed, %d dropped\n", total, stats.routed, stats.dropped);
    printf("  Bytes     : %d routed, %d dropped\n", stats.bytes_routed, stats.bytes_dropped);
    printf("  Priority  : %d HIGH, %d LOW\n", stats.packet_count_by_type[HIGH], stats.packet_count_by_type[LOW]);
    printf("\n");
    for (int i = 0; i < 4; i++)
        printf("  eth%d : %d packets%s\n", i, stats.eth_counts[i], i == busiest_eth ? " <- busiest" : "");
    printf("-----------------------------------------------------------------\n");
}