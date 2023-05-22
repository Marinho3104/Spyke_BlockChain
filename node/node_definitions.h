

#ifndef NODE_NODE_DEFINITIONS_H
#define NODE_NODE_DEFINITIONS_H

// Node Connections Information related

// Params lengths
#define NODE_NODE_CONNECTIONS_INFORMATION_INITIAL_CONNECTIONS_COUNT_LENGTH 2


// Node Information related

// Params lengths
#define NODE_NODE_INFORMATION_MAX_SERVER_OPEN_CONNECTIONS_LENGTH 2
#define NODE_NODE_INFORMATION_MAX_ORDINARY_CONNECTIONS_LENGTH 2
#define NODE_NODE_INFORMATION_MAX_STABLE_CONNECTIONS_LENGTH 2
#define NODE_NODE_INFORMATION_MEMORY_POOL_SIZE_LENGTH 8

#define NODE_NODE_INFORMATION_ORDINARY_CONNECTION 0
#define NODE_NODE_INFORMATION_STABLE_CONNECTION 1

// Node Related

// When not specify path, this is used as path
#define NODE_NODE_DEFINITIONS_DEFAULT_SAVE_PATH "./node_data"

#endif