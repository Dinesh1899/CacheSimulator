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

CACHEBLOCK CACHEMEMORY::get_read_block(int addr){
    CACHEBLOCK cb;
    int tag_lsb = this->block_offset_bits_length + this->index_bits_length;
    cb.tag = this->get_masked_data(addr, this->tag_mask, tag_lsb);
    cb.block_offset = this->get_masked_data(addr, this->block_offset_mask, 0);
    cb.counter = 0;
    cb.is_dirty = false;
    cb.is_valid = true;

    return cb;
}


void insert_block_and_update_lru(vector<CACHEBLOCK>& line, int idx, CACHEBLOCK& new_block){
    CACHEBLOCK existing_block = line.at(idx);
    int lru = existing_block.counter;

    line.erase(line.begin()+idx);

    for(CACHEBLOCK block : line){
        if(block.counter < lru){
            block.counter++;
        }
    }

    // insert new block
    line.push_back(new_block);
}

int search_block_by_tag(vector<CACHEBLOCK>& line, int tag){
    int hit_block_idx = -1;
    int idx = 0;

    for(CACHEBLOCK block : line){
        if(block.is_valid && block.tag == tag){
            return idx;
        }
        idx++;
    }
    return -1;
}

bool is_cache_full(vector<CACHEBLOCK>& line){
    int idx = 0;

    for(CACHEBLOCK block : line){
        if(block.is_valid){
            return idx;
        }
        idx++;
    }
    return idx == line.size();
}

bool get_lru_block(vector<CACHEBLOCK>& line){
    int lru_idx = -1;
    int idx = 0;
    for(CACHEBLOCK block : line){
        if(block.is_valid && block.counter > lru_idx){
            lru_idx = block.counter;
        }
        idx++;
    }
    return lru_idx;
}

bool CACHEMEMORY::read_request(int addr){
    // Get the cache line from addr and index mask
    int idx = get_masked_data(addr, this->index_mask, this->block_offset_bits_length);
    vector<CACHEBLOCK> line = this->cache[idx];

    // Search block from the cacheline
    CACHEBLOCK requested_block = this->get_read_block(addr);
    int block_idx = search_block_by_tag(line, requested_block.tag);

    if(block_idx == -1){
        // Read Miss
        // fetch from next level 
        // insert new block
        // update lru
        // return false

        if(is_cache_full(line)){
            // line is full --> evict block
        }
        // fetch from next level
        if(this->next_mem != nullptr){
            this->next_mem->read_request(addr);
        }

        // insert new block
        // update lru
        insert_block_and_update_lru(line, line.size(), requested_block)
        // return false 
    }else{
        // Read Hit --> return true and update lru counter
        insert_block_and_update_lru(line, block_idx, requested_block);
    }
    


}
