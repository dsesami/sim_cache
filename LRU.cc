#include <iostream>
#include "sim_cache.h"

bool Cache::LRU(unsigned long long index, unsigned long long tag, char operation) {
    
    bool writeback;
    size_t replace = 0;

    for(size_t i=0; i<number_of_ways; i++) {
        if(this->L1[index][i].priority == (number_of_ways - 1)) {
            replace = i;
        }
        else this->L1[index][i].priority++;
    }

    this->L1[index][replace].tag = tag;
    this->L1[index][replace].priority = 0;
    //cout << "replaced: " << replace << endl;
    if(this->L1[index][replace].dirty_bit == true) writeback = true;
    else writeback = false;

    if(operation == 'w') this->L1[index][replace].dirty_bit = true;
    else this->L1[index][replace].dirty_bit = false;

    return writeback;
}
