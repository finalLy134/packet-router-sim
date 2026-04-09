# packet-router-sim
An advanced packet router simulator made to simulate a real router's routing process. I had to learn Networking and Subnets for this project so it was really interesting.

## Features
- Trie implementation with Longest Prefix Match (LPM)
- Max Heap Priority Queue implementation
- IP Parsing
- Routing with Subnet awareness
- Routing Statistics
- Input received from easily configurable files

## Input Format
`routes.txt`:
```d
<IP>/<CIDR> <ETH>
```

`packets.txt`:
```d
<IP_SRC> <IP_DEST> <PRIORITY:HIGH/LOW> <SIZE>
```

## Overview
This packet router simulator models a real router using efficient data structures.  
Routes are stored in a Trie and processed with Longest Prefix Match (LPM), while packets are prioritized using a Max Heap Priority Queue.
We have 2 main data structures currently.
- Trie
- Max Heap Priority Queue

The Trie contains all routes mapped by the 32 bits of the IP Address, of course not all bits are being used because we have a Subnet mask that groups IP Addresses together.

The Max Heap Priority Queue contains all packets ordered by their priority type. Currently HIGH and LOW.

After we understand what does these data structures do in our project, everything becomes simple.
We start by loading all routes from the `routes.txt` into the Trie. Afterwards we load all packets from the `packets.txt` into our Priority Queue.
And then we can start using our router. In the current implementation, we just process all packets with the given routes and print the statistics of the process.

This process can be seen here (`main.c`):
```c
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
```

Example output:
```
[PACKET #001] 192.168.1.5      -> 10.0.0.1         -> eth2   | Priority: HIGH | Size:   64 | ROUTED
[PACKET #002] 10.0.0.5         -> 192.168.1.1      -> eth0   | Priority: HIGH | Size:   64 | ROUTED
[PACKET #003] 8.8.4.4          -> 1.1.1.1          -> eth3   | Priority: HIGH | Size:   32 | ROUTED
[PACKET #004] 172.16.0.3       -> 10.0.0.1         -> eth2   | Priority: HIGH | Size:   32 | ROUTED
[PACKET #005] 192.168.1.20     -> 10.0.0.2         -> eth2   | Priority: LOW  | Size:  128 | ROUTED
[PACKET #006] 172.16.5.10      -> 10.0.0.3         -> eth2   | Priority: LOW  | Size:  256 | ROUTED
[PACKET #007] 8.8.8.8          -> 1.1.1.1          -> eth3   | Priority: LOW  | Size:  512 | ROUTED
[PACKET #008] 203.0.113.7      -> 10.0.0.1         -> eth2   | Priority: LOW  | Size:  128 | ROUTED

--- Routing Stats -----------------------------------------------
  Packets   : 8 total, 8 routed, 0 dropped
  Bytes     : 1216 routed, 0 dropped
  Priority  : 4 HIGH, 4 LOW

  eth0 : 1 packets
  eth1 : 0 packets
  eth2 : 5 packets <- busiest
  eth3 : 2 packets
-----------------------------------------------------------------
```

## Special Thanks
Shoutout to the awesome teachers who made these videos — I couldn’t have done this project without them!
* [Tushar Roy's Trie Data Structure YouTube Video](https://youtu.be/AXjmTQ8LEoI?si=5PhHClCiJmjsD9zR)
* [Practical Networking's Subnetting Mastery Series](https://www.youtube.com/watch?v=BWZ-MHIhqjM&list=PLIFyRwBY_4bQUE4IB5c4VPRyDoLgOdExE)
