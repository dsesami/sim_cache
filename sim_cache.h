// cache.h

#ifndef __CACHE_INCLUDED__
#define __CACHE_INCLUDED__

#include <iostream>
#include <string>
#include <bitset>
#include <vector>

struct cache_block {
    unsigned long long tag; // comparator for hits and misses
    unsigned int priority;  // determines the result of replacement
    bool dirty_bit; // comparator for writeback
    bool full; // one-time state indicator to indicate if block is full
    cache_block() : dirty_bit(false), full(false) {}
};

class Cache {
    public:
        unsigned int number_of_sets;
        unsigned int number_of_ways;
        unsigned int address_bits;
        unsigned int tag_width;
        unsigned int index_width;
        unsigned int offset_width;
        unsigned int policy;
        std::vector< std::vector<int> > pTree; 
        cache_block ** L1;

        Cache(unsigned long long BLOCKSIZE, unsigned long long L1_SIZE,
                     int L1_ASSOC, int REPL_POLICY,
                     int INCLUSION, std::string TRACE_FILE);
        
        unsigned int process(unsigned long long hex_address, char operation);
        unsigned int store(unsigned long long index, unsigned long long tag, char operation);
        
        void LRUhit(unsigned long long index, size_t pos);
        bool LRU(unsigned long long index, unsigned long long tag, char operation);
        bool FIFO(unsigned long long index, unsigned long long tag, char operation);
        bool pLRU(unsigned long long index, unsigned long long tag, char operation);
        void pLRUarrange(unsigned long long index, std::vector<int> binary_way);
        void pLRUstatus();
        std::vector<int> wayToBits(int i);
        
        ~Cache();
};

#endif 
