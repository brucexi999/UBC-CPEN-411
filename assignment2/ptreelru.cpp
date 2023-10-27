//--- Implementing the Pseudo-Tree-Based LRU Replacement Policy ---//
// Implemented with reference to https://www.cs.virginia.edu/luther/3330/S2022/tree-plru.html#fnref1
#include <algorithm>
#include <iostream>
#include "cache.h"

#define TREE_SIZE  LLC_WAY-1
#define TREE_DEPTH (int)log2(LLC_WAY)
bool plru_tree[LLC_SET][TREE_SIZE];

// initialize replacement state
void CACHE::llc_initialize_replacement()
{
    // Initialize PLRU trees
    for(int i=0; i<LLC_SET; i++) {
        for(int j=0; j<TREE_SIZE; j++) {
            plru_tree[i][j] = 0;  // Initially, all bits are set to 0.
        }
    }
}

// find replacement victim
uint32_t CACHE::llc_find_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    // Forward pass of the tree following the arrows
    uint32_t lru_way = 0;
    int node = 0;
    int i = 0;
    int lru_node[TREE_DEPTH];

    while (node<TREE_SIZE) {
        lru_node[i] = plru_tree[set][node];
        node = 2*node+1+lru_node[i];
        i++;
    }

    for (i=0; i<TREE_DEPTH; i++) {
        lru_way = 2*lru_way+lru_node[i];
    }

    return lru_way;
}

// called on every cache hit and cache fill
void CACHE::llc_update_replacement_state(uint32_t cpu, uint32_t set, uint32_t way, uint64_t full_addr, uint64_t ip, uint64_t victim_addr, uint32_t type, uint8_t hit)
{
    // Backward track of the tree to flip the bits
    int node = 0;
    int mru_node[TREE_DEPTH];

    // Convert 'way' to binary and store the bits in the mru_node array
    for (int i=0; i<=TREE_DEPTH; i++) {
        mru_node[i] = (way >> i) & 1;
    }

    std::reverse(std::begin(mru_node), std::end(mru_node));

    // Flipping the bits of the traversed nodes
    for (int i=0; i<TREE_DEPTH; i++) {
        if (plru_tree[set][node] == mru_node[i]) {
            plru_tree[set][node] = !plru_tree[set][node]; // Flip the bit along the MRU path
        }
        node = 2*node+1+mru_node[i];
    }
}

void CACHE::llc_replacement_final_stats()
{

}
