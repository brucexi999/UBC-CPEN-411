//--- Implementing the Dynamic Insertion Policy ---//

#include "cache.h"
#include <random>

#define NUM_POLICY 2
#define SDM_SIZE 64
#define TOTAL_SDM_SETS NUM_CPUS*NUM_POLICY*SDM_SIZE
#define PSEL_WIDTH 10
#define PSEL_MAX 1023

uint32_t PSEL[NUM_CPUS];
unsigned rand_sets[TOTAL_SDM_SETS];

// initialize replacement state
void CACHE::llc_initialize_replacement()
{
    srand(time(NULL));
    unsigned long rand_seed = 1;
    unsigned long max_rand = 1048576;
    uint32_t my_set = LLC_SET;
    int do_again = 0;
    for (int i=0; i<TOTAL_SDM_SETS; i++) {
        do {
            do_again = 0;
            rand_seed = rand_seed * 1103515245 + 12345;
            rand_sets[i] = ((unsigned) ((rand_seed/65536) % max_rand)) % my_set;
            printf("Assign rand_sets[%d]: %u  LLC: %u\n", i, rand_sets[i], my_set);
            for (int j=0; j<i; j++) {
                if (rand_sets[i] == rand_sets[j]) {
                    do_again = 1;
                    break;
                }
            }
        } while (do_again);
        printf("rand_sets[%d]: %d\n", i, rand_sets[i]);
    }

    for (int i=0; i<NUM_CPUS; i++)
        PSEL[i] = 0;
}

int is_it_leader(uint32_t cpu, uint32_t set)
{
    uint32_t start = cpu * NUM_POLICY * SDM_SIZE,
             end = start + NUM_POLICY * SDM_SIZE;

    for (uint32_t i=start; i<end; i++)
        if (rand_sets[i] == set)
            return ((i - start) / SDM_SIZE);

    return -1;
}

int generate_randnum(void)
{
    int random_num;

    std::random_device rd;
    std::uniform_int_distribution<int> dis(0, 99);
    random_num = dis(rd);
    
    return random_num;
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
        int leader = is_it_leader(cpu, set);

        if (leader == -1) { // the set is a follower
            if (PSEL[cpu] >= 512) { // 10-bit PSEL, when MSB = 1 and others 0, it is 'd512, follow BIP
                random_num = generate_randnum();
                if (random_num < 5) {
                    return lru_update(set, way);
                }
                else {
                    return;
                }
            }
            else if (PSEL[cpu] <= 511) { // follow LRU
                return lru_update(set, way);
            }
        }
        else if (leader == 0) { // leader 0: BIP
            if (PSEL[cpu] > 0) {
                PSEL[cpu] --;
            }
            random_num = generate_randnum();
            if (random_num < 5) {
                return lru_update(set, way);
            }
            else {
                return;
            }
        }
        else if (leader == 1) { // leader 1: LRU
            if (PSEL[cpu] < PSEL_MAX) {
                PSEL[cpu] ++;
            }
            return lru_update(set, way);
        }
    }
}

void CACHE::llc_replacement_final_stats()
{

}
