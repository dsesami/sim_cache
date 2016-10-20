#include <iostream>
#include <cmath>
#include "sim_cache.h"
#include <vector>

using namespace std;
bool Cache::pLRU(unsigned long long index, unsigned long long tag, char operation) {

    bool writeback = true;

    size_t steps = log2(number_of_ways);
    size_t current = 1;

    // navigate your binary tree array
    vector<int> binary_way;
    for(size_t i=0; i < steps; i++) {
            binary_way.push_back(this->pTree[index][current]);
            current = this->pTree[index][current] ? (current * 2 + 1) : (current * 2);
    }
    
    Cache::pLRUarrange(index, binary_way);
    current = current - number_of_ways;

    this->L1[index][current].tag = tag;
    
    if(this->L1[index][current].dirty_bit == true) writeback = true;
    else writeback = false;

    if(operation == 'w') this->L1[index][current].dirty_bit = true;
    else this->L1[index][current].dirty_bit = false;

    //cout << "we replaced way number: " << current << endl;
    return writeback;
}

// rearranges the current binary tree
void Cache::pLRUarrange(unsigned long long index, vector<int> binary_way) {
	// compares the binary request to the current code
    int pos = 0;
    int current = 1;

    //cout << "binary way is ";
    //for(size_t i = 0; i< binary_way.size(); i++)
    //    cout << binary_way[i];
    //cout << endl;
    //cout << "before arranging: ";
    //Cache::pLRUstatus();
    
    for(size_t j=0; j < binary_way.size(); j++) {
        if (this->pTree[index][current] == binary_way[pos])
                this->pTree[index][current] = !this->pTree[index][current];
        
        current = binary_way[pos] ? (current * 2 + 1) : (current * 2);
        pos++;
    }

   // cout << "after: ";
   // Cache::pLRUstatus();
}

// status of the pLRU binary trees
void Cache::pLRUstatus() {
    for(size_t i=0;i<number_of_sets; i++) {
        for(size_t j=1;j<number_of_ways;j++)
            cout << this->pTree[i][j];
        cout << endl;
    }
}

// function which converts the current way to a vector<int>
vector<int> Cache::wayToBits(int i) {
    int steps = log2(number_of_ways);

    vector<int> binary_way;
    while(steps != 0) {
        binary_way.push_back(i % 2);
        i =  i / 2;
        steps--;
    }
    return binary_way;
}
