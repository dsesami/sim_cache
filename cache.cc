/**
 * Cache class
 * @author Daniel Albert Sami
 * Fall 2016
 *
 * This class defines the cache itself, as well as any different operations that might occur on it.
 */
#include <iostream>
#include <string>
#include <math.h>
#include "sim_cache.h"
#include <bitset>
#include <vector>

using namespace std;

/** Cache constructor. Allocates space and defines dimensions.*/
Cache::Cache(unsigned long long BLOCKSIZE, unsigned long long L1_SIZE, 
             int L1_ASSOC, int REPL_POLICY, 
             int INCLUSION, string TRACE_FILE) {

    address_bits = 64; // same every time
    number_of_ways = L1_ASSOC;
    policy = REPL_POLICY;

    // Constructs a new cache
    number_of_sets = L1_SIZE / (L1_ASSOC * BLOCKSIZE);
    offset_width   = log2(BLOCKSIZE);
    index_width    = log2(number_of_sets);
    tag_width      = address_bits - index_width - offset_width;

    this->pTree = vector< vector <int> > (number_of_sets);
    for(size_t i =0; i<number_of_sets; i++)
        for(size_t j =0; j<number_of_ways; j++)
            this->pTree[i].push_back(0);

    L1 = new cache_block * [number_of_sets];
    for(size_t i=0; i<number_of_sets; i++) {
        L1[i] = new cache_block[L1_ASSOC];
    }
}

/* Parses address and calls the correct processes */
unsigned int Cache::process(unsigned long long hex_address, char operation) {

    unsigned long long tag = 0, index = 0, offset = 0;
    bitset<64> request = (hex_address);
    unsigned int result;

    // extract offset
    unsigned long long bitmask = 1;
    for(size_t i=0; i<offset_width; i++) {
        if (request.test(i)) offset |= bitmask;
        bitmask <<= 1;
    }
    
    // extract index
    bitmask = 1;
    for(size_t i=offset_width; i<(offset_width+index_width); i++) {
        if (request.test(i)) index |= bitmask;
        bitmask <<= 1; 
    }

    // extract tag
    bitmask = 1;
    for(size_t i = (offset_width+index_width); i<64; i++) {
        if (request.test(i)) tag |= bitmask;
        bitmask <<=1;
    }

    // access the cache
    result = this->store(index, tag, operation);

    // process result 
    if (operation == 'r') {
        switch(result) {
            case 1: return 1; // hit
            case 2: return 2; // miss
            case 3: return 3; // miss and writeback
        }
    }
    else if (operation == 'w'){
        switch(result) {
            case 1: return 4; // hit
            case 2: return 5; // miss
            case 3: return 6; // miss and writeback
        }
    }
    return 0;
}

/* Destructor to deallocate the cache */
Cache::~Cache(){
    for(size_t i=0; i<number_of_sets; i++) {
        delete[] L1[i];
    }
    delete[] L1;
}
