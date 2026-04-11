# packet-router-sim
An advanced packet router simulator made to simulate a real router's routing process. I had to learn Networking and Subnets for this project so it was really interesting.

## Features
- Trie implementation with Longest Prefix Match (LPM)
- Max Heap Priority Queue implementation
- Hash Table implementation to serve as an ARP Table
- IP Parsing
- Routing using Longest Prefix Match (CIDR-based routing)
- Routing Statistics
- Input received from easily configurable files

## Input Format
`routes.txt`:
```d
<IP>/<CIDR> <ETH>
```

`devices.txt`:
```d
<IP> <MAC_ADDRESS>
```

`packets.txt`:
```d
<IP_SRC> <IP_DEST> <PRIORITY:HIGH/LOW> <SIZE>
```

## Overview
This packet router simulator models a real router using efficient data structures.  
Routes are stored in a Trie and processed with Longest Prefix Match (LPM), while packets are prioritized using a Max Heap Priority Queue.
Devices MAC Addresses are mapped in a Hash Table acting as a simplified ARP-like table
We have 3 main data structures currently.
- Trie
- Max Heap Priority Queue
- Hash Table

The Trie stores routes based on IP prefixes (CIDR-based) for Longest Prefix Match (LPM).

The Max Heap Priority Queue contains all packets ordered by their priority type. Currently HIGH and LOW.

The Hash Table contains all device ip addresses as keys and their physical MAC address as value.

After we understand what all of these data structures do in our project, the routing flow becomes straightforward.
We start by loading all routes from the `routes.txt` into the Trie, then we load all packets from the `packets.txt` into our Priority Queue. 
In addition loading all devices MAC addresses from the `devices.txt` into our Hash Table.
And then we can start using our router. In the current implementation, we just process all packets with the given routes and print the statistics of the process.

This process can be seen here (`main.c`):
```c
#include "router.h"

int main() {
	trie_node* routes = load_routes("routes.txt");
	priority_queue* packets = load_packets("packets.txt");
	hashtable* devices = load_devices("devices.txt");

	router_stats stats = process_packets(packets, routes, devices);

	print_stats(stats);

	free_trie(routes);
	free_heap(packets);
	free_hashtable(devices);

	return 0;
}
```

Example output:
```
[PACKET #001] | 192.168.1.5      -> 10.0.0.1         | ETH=eth2 MAC=3A:7F:91:2C:4D:88 | HIGH | 64    | DELIVERED
[PACKET #002] | 10.0.0.5         -> 192.168.1.1      | ETH=eth0 MAC=A2:63:0F:BE:94:21 | HIGH | 64    | DELIVERED
[PACKET #003] | 8.8.4.4          -> 1.1.1.1          | ETH=eth3 MAC=5D:E8:33:6A:C0:7F | HIGH | 32    | DELIVERED
[PACKET #004] | 172.16.0.3       -> 10.0.0.1         | ETH=eth2 MAC=3A:7F:91:2C:4D:88 | HIGH | 32    | DELIVERED
[PACKET #005] | 192.168.1.20     -> 10.0.0.2         | ETH=eth2 MAC=6E:B2:45:9A:11:CF | LOW  | 128   | DELIVERED
[PACKET #006] | 172.16.5.10      -> 10.0.0.3         | ETH=eth2 MAC=8C:19:D4:77:2E:5B | LOW  | 256   | DELIVERED
[PACKET #007] | 8.8.8.8          -> 1.1.1.1          | ETH=eth3 MAC=5D:E8:33:6A:C0:7F | LOW  | 512   | DELIVERED
[PACKET #008] | 203.0.113.7      -> 10.0.0.1         | ETH=eth2 MAC=3A:7F:91:2C:4D:88 | LOW  | 128   | DELIVERED

--- Routing Stats -----------------------------------------------
  Packets  : TOTAL=8 | DELIVERED=8 | NO_MAC=0 | DROPPED=0
  Bytes    : DELIVERED=1216 | NO_MAC=0 | DROPPED=0
  Priority : HIGH=4 | LOW=4

  Interfaces:
    eth0 : 1
    eth1 : 0
    eth2 : 5    <- busiest
    eth3 : 2
-----------------------------------------------------------------
```

## Special Thanks
Shoutout to the awesome teachers who made these videos — I couldn’t have done this project without them!
* [Tushar Roy's Trie Data Structure YouTube Video](https://youtu.be/AXjmTQ8LEoI?si=5PhHClCiJmjsD9zR)
* [Practical Networking's Subnetting Mastery Series](https://www.youtube.com/watch?v=BWZ-MHIhqjM&list=PLIFyRwBY_4bQUE4IB5c4VPRyDoLgOdExE)
