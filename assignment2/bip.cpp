//--- Implementing the Bimodal Insertion Policy ---//

#include "cache.h"
#include <random>

// initialize replacement state
void CACHE::llc_initialize_replacement()
{

}

// find replacement victim
uint32_t CACHE::llc_find_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    // baseline LRU
    return lru_victim(cpu, instr_id, set, current_set, ip, full_addr, type); 
}

// called on every cache hit and cache fill
void CACHE::llc_update_replacement_state(uint32_t cpu, uint32_t set, uint32_t way, uint64_t full_addr, uint64_t ip, uint64_t victim_addr, uint32_t type, uint8_t hit)
{   
    int random_num;

    if (hit && (type == WRITEBACK)) // writeback hit does not update LRU state
        return;
    else if (hit) {
        return lru_update(set, way);
    }
    else if (!hit) {
        // The newly filled cache line will remain LRU with a probability of 95%
        // Other lines's LRU will remain the same
        std::random_device rd;
        std::uniform_int_distribution<int> dis(0, 99);
        random_num = dis(rd);
        if (random_num < 5) {
            return lru_update(set, way);
        }
        else {
            return;
        }
    }
}

void CACHE::llc_replacement_final_stats()
{

}