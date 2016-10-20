/**
 * process.cc
 * Daniel Sami Fall 2016
 * Contains all functions operating on the cache minus replacement policies
 */

#include <iostream>
#include "sim_cache.h"
#include <inttypes.h>
#include <cmath>
#include <vector>
using namespace std;

#define HIT 1
#define MISS 2
#define MISS_AND_WRITEBACK 3

/* store something in the cache */
unsigned int Cache::store(unsigned long long index, unsigned long long tag, char operation) {

    bool writeback;
    vector<int> binary_way;
    for(size_t i=0; i<number_of_ways; i++) {

        // Check for a hit. If the block is full move to the next one
        if(this->L1[index][i].full == true) {
            if(this->L1[index][i].tag == tag) {
                // a hit!
                
                // pLRU extract way requested
                binary_way = wayToBits(i);

                if(operation == 'w') this->L1[index][i].dirty_bit = true;

                // rearrange for LRU
                if(policy == 0) {
                    for(size_t j=0; j<number_of_ways;j++) {
                        if(j != i) {
                            if(this->L1[index][j].priority < this->L1[index][i].priority)
                                this->L1[index][j].priority++;
                        }
                    }
                    this->L1[index][i].priority = 0;
                }
                //pLRU rearrangement
                if(policy == 2) {
                	Cache::pLRUarrange(index, binary_way);
                }

                return HIT;
            }
            
            // are we at the last way possible?
            if(i == (number_of_ways-1)) {
                // register a miss and enact replacement policy
                if (policy == 0) writeback = Cache::LRU(index, tag, operation);
                if (policy == 1) writeback = Cache::FIFO(index, tag, operation);
                if (policy == 2) writeback = Cache::pLRU(index, tag, operation);
                if (writeback) return MISS_AND_WRITEBACK;
                else return MISS;
            }
            else {
                continue;
            }
        }

        // Fill the tag and register a cold miss

        binary_way = wayToBits(i);
        if(policy == 0) {
            this->L1[index][i].priority = 0;
            for(size_t j=0; j<number_of_ways; j++)
                if(this->L1[index][j].full && j!=i)
                    this->L1[index][j].priority++;
        }

        else if(policy == 1) 
            this->L1[index][i].priority = i;

        else if(policy == 2)
            Cache::pLRUarrange(index, binary_way);
        this->L1[index][i].tag  = tag;
        this->L1[index][i].full = true;
        if (operation == 'w') this->L1[index][i].dirty_bit = true;

        return MISS;
    }
    // signal invalid state
    return 0;
}
