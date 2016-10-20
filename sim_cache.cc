/* 
 * sim_cache
 * @author Daniel Albert Sami
 * ECE 463
 * Fall 2016
 *
 * Project #1
 * ==========
 * L1 cache simulator with support for LRU, FIFO, and pseudo-LRU replacement policies.
 *
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include "sim_cache.h"

#define INVALID          0
#define READ             1
#define READ_MISS        2
#define R_MISS_WRITEBACK 3
#define WRITE            4
#define W_MISS           5
#define W_MISS_WRITEBACK 6

using namespace std;

int main(int argc, char * argv[]) {

    if (argc != 9) {
       cout << "You need 8 arguments. You currently have: " << argc-1 << endl;
        return 1;
    }

    char operation;
    string address; 
    unsigned long long hex_address;
    unsigned int result = 0;
    string replacement, inclusion;

    // Command line variables.
    unsigned long long BLOCKSIZE, L1_SIZE, L2_SIZE, L2_ASSOC;
    int L1_ASSOC, REPL_POLICY, INCLUSION;
    BLOCKSIZE    = atof(argv[1]); // Block size in bytes
    L1_SIZE      = atof(argv[2]); // L1 cache size in bytes
    L1_ASSOC     = atof(argv[3]); // L1 set-associativity (1 = direct-mapped)
    L2_SIZE      = atof(argv[4]); // L2 cache size in bytes (0 = no L2 cache)
    L2_ASSOC     = atof(argv[5]); // L2 set-associativity
    REPL_POLICY  = atof(argv[6]); // 0 = LRU, 1 = FIFO, 2 = pseudoLRU
    INCLUSION    = atof(argv[7]); // 0 = non-inclusive, 1 = inclusive, 2 = exclusive
    switch(REPL_POLICY) {
        case 0: replacement = "LRU"; break;
        case 1: replacement = "FIFO"; break;
        case 2: replacement = "Pseudo"; break;
    }
    switch(INCLUSION) {
        case 0: inclusion = "non-inclusive"; break;
        case 1: inclusion = "inclusive"; break;
        case 2: inclusion = "exclusive"; break;
    }
    
    const char * TRACE_FILE    = argv[8]; // Full name of trace file (example: trace_1.txt).

    
    // L1 cache result values.
    unsigned long long L1_reads      = 0;
    unsigned long long L1_r_misses   = 0;
    unsigned long long L1_writes     = 0;
    unsigned long long L1_w_misses   = 0;
    double L1_missrate   = 0;
    unsigned long long L1_writebacks = 0;

    // L2 cache simulation is currently not implemented.
    unsigned long long L2_reads      = 0;
    unsigned long long L2_r_misses   = 0;
    unsigned long long L2_writes     = 0;
    unsigned long long L2_w_misses   = 0;
    double L2_missrate   = 0;
    unsigned long long L2_writebacks = 0;

    // Total traffic passed through system
    unsigned long long total_traffic = 0;

    Cache * c = new Cache(BLOCKSIZE, L1_SIZE, L1_ASSOC, REPL_POLICY, INCLUSION, TRACE_FILE);

	// filestream
    ifstream infile(TRACE_FILE);

    // get one line at a time
    string line;
    while(getline(infile, line)) {
        
        istringstream iss(line);
        stringstream ss; // for conversions
        
        // read in read/write and address
        if(!(iss >> operation >> address)) break;

        // get a usable hex address
        address = "0x" + address;
        sscanf(address.c_str(), "%016llX", &hex_address);
       // cout << operation << " "  << hex <<  hex_address << endl;
        result = c->process(hex_address, operation);
       
        switch(result) {
            case 0:// invalid
                cout << "invalid state"; return 1;
            case 1:// read hit
                L1_reads++; break;
            case 2:// read miss
                L1_reads++;
                L1_r_misses++; break;
            case 3:// read miss with writeback
                L1_reads++;
                L1_r_misses++;
                L1_writebacks++;
                break;
            case 4:// write hit
                L1_writes++; break;
            case 5:// write miss
                L1_writes++;
                L1_w_misses++; break;
            case 6:// write miss with writeback
                L1_writes++;
                L1_w_misses++;
                L1_writebacks++;
                break;
        }
    }
 
    total_traffic = L1_r_misses + L1_w_misses + L1_writebacks;
    L1_missrate = (double)(L1_r_misses + L1_w_misses) / (double)(L1_reads + L1_writes);

    // Let's print out our final results.
    cout << 
        "===== Simulator configuration ====="     << endl <<
        "BLOCKSIZE:             " << BLOCKSIZE    << endl <<
        "L1_SIZE:               " << L1_SIZE      << endl <<
        "L1_ASSOC:              " << L1_ASSOC     << endl <<
        "L2_SIZE:               " << L2_SIZE      << endl <<
        "L2_ASSOC:              " << L2_ASSOC     << endl <<
        "REPLACEMENT POLICY:    " << replacement  << endl <<
        "INCLUSION PROPERTY:    " << inclusion    << endl <<
        "trace_file:            " << TRACE_FILE   << endl <<
        "===== Simulation results (raw) ====="            << endl <<
        "a. number of L1 reads:        " << dec << L1_reads      << endl <<
        "b. number of L1 read misses:  " << L1_r_misses   << endl <<
        "c. number of L1 writes:       " << L1_writes     << endl <<
        "d. number of L1 write misses: " << L1_w_misses   << endl <<
        "e. L1 miss rate:              " << L1_missrate   << endl <<
        "f. number of L1 writebacks:   " << L1_writebacks << endl <<
        "g. number of L2 reads:        " << L2_reads      << endl <<
        "h. number of L2 read misses:  " << L2_r_misses   << endl <<
        "i. number of L2 writes:       " << L2_writes     << endl <<
        "j. number of L2 write misses: " << L2_w_misses   << endl <<
        "k. L2 miss rate:              " << L2_missrate   << endl <<
        "l. number of L2 writebacks:   " << L2_writebacks << endl <<
        "m. total memory traffic:      " << total_traffic << endl;

    // Destroy cache object
    delete c;
    // Complete.
    return 0;
}

