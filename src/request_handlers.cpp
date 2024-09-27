#include "cache_mem.cpp"

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <math.h>

using namespace std;

int CACHEMEMORY::get_masked_data(int addr, int mask, int start){
    return (addr & mask) >> start;
}

bool CACHEMEMORY::read_request(int addr){
    // Get the cache line from addr and index mask
    int idx = get_masked_data(addr, this->index_mask, this->block_offset_bits_length);

}
