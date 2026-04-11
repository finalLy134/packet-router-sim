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

hashtable* load_devices(char* file_name) {
    FILE* fp = fopen(file_name, "r");

    if (fp == NULL) {
        printf("Could not open file %s for device loading.", file_name);
        return NULL;
    }

    char buff[128];

    hashtable* table = create_hashtable();

    if (table == NULL) {
        printf("Out of memory.\n");
        fclose(fp);
        return NULL;
    }

    while (fgets(buff, sizeof(buff), fp)) {
        char device_ip[32], mac[18];

        sscanf_s(buff, "%s %s", device_ip, 32, mac, 18);
        put(table, parse_ip(device_ip), mac);
    }

    fclose(fp);
    return table;
}

packet_result resolve_packet(packet* cur, trie_node* root, hashtable* devices, char* eth_out, char* mac_out) {
    packet_result result;
    
    char* eth = search(root, cur->dest);
    char* mac = get(devices, cur->dest);

    result.has_route = (eth != NULL);
    result.has_mac = (mac != NULL);

    if (eth) strcpy_s(eth_out, 16, eth);
    else     strcpy_s(eth_out, 16, "NONE");

    if (mac) strcpy_s(mac_out, MAC_SIZE, mac);
    else     strcpy_s(mac_out, MAC_SIZE, "NONE");

    return result;
}

void print_packet(int count, packet* cur, const char* eth, const char* mac, packet_result res) {
    char src[32], dest[32];
    stringify_ip(cur->src, src, sizeof(src));
    stringify_ip(cur->dest, dest, sizeof(dest));

    const char* status;

    if (!res.has_route)     status = "DROPPED";
    else if (!res.has_mac)  status = "NO_MAC";
    else                    status = "DELIVERED";

    const char* priority = cur->priority == HIGH ? "HIGH" : "LOW";

    printf("[PACKET #%03d] | %-16s -> %-16s | ETH=%-4s MAC=%-17s | %-4s | %-5d | %s\n",
        count,
        src,
        dest,
        eth,
        mac,
        priority,
        cur->size,
        status
    );
}

void update_stats(router_stats* stats, packet* cur, const char* eth, packet_result res) {
    if (!res.has_route) {
        stats->dropped++;
        stats->bytes_dropped += cur->size;
    }
    else {
        int eth_index = eth[3] - '0';
        stats->eth_counts[eth_index]++;

        if (res.has_mac) {
            stats->delivered++;
            stats->bytes_delivered += cur->size;
        }
        else {
            stats->no_mac++;
            stats->bytes_no_mac += cur->size;
        }
    }

    stats->packet_count_by_type[cur->priority]++;
}

router_stats process_packets(priority_queue* pq, trie_node* root, hashtable* devices) {
    router_stats stats = { 0 };
    int count = 1;

    while (!heap_is_empty(pq)) {
        packet cur = dequeue(pq);

        char eth[16];
        char mac[MAC_SIZE];

        packet_result result = resolve_packet(&cur, root, devices, eth, mac);
        print_packet(count, &cur, eth, mac, result);
        update_stats(&stats, &cur, eth, result);

        count++;
    }

    return stats;
}

void print_stats(router_stats s) {
    int total = s.dropped + s.no_mac + s.delivered;

    int busiest = 0;
    for (int i = 1; i < 4; i++)
        if (s.eth_counts[i] > s.eth_counts[busiest])
            busiest = i;

    printf("\n--- Routing Stats -----------------------------------------------\n");

    printf("  Packets  : TOTAL=%d | DELIVERED=%d | NO_MAC=%d | DROPPED=%d\n",
        total, s.delivered, s.no_mac, s.dropped);

    printf("  Bytes    : DELIVERED=%d | NO_MAC=%d | DROPPED=%d\n",
        s.bytes_delivered, s.bytes_no_mac, s.bytes_dropped);

    printf("  Priority : HIGH=%d | LOW=%d\n",
        s.packet_count_by_type[HIGH],
        s.packet_count_by_type[LOW]);

    printf("\n  Interfaces:\n");

    for (int i = 0; i < 4; i++)
        printf("    eth%d : %-4d %s\n",
            i,
            s.eth_counts[i],
            (i == busiest ? "<- busiest" : ""));

    printf("-----------------------------------------------------------------\n");
}