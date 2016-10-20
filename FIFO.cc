#include <iostream>
#include "sim_cache.h"

bool Cache::FIFO(unsigned long long index, unsigned long long tag, char operation) {
    
    bool writeback;

    for(size_t i=0; i<number_of_ways; i++) {
        if(this->L1[index][i].priority == 0) {
            this->L1[index][i].priority = number_of_ways - 1;

            if(this->L1[index][i].dirty_bit) writeback = true;
            else writeback = false;
            
            if(operation == 'w') {
                this->L1[index][i].dirty_bit = true; 
            }
            else {
                this->L1[index][i].dirty_bit = false;
            }
            this->L1[index][i].tag = tag;
        }
        else this->L1[index][i].priority--;
    }
    return writeback;
}
