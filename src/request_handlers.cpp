#include "cache_mem.cpp"

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <math.h>

using namespace std;

unsigned int CACHEMEMORY::get_masked_data(unsigned int addr, unsigned int mask, int start){
    return (addr & mask) >> start;
}

CACHEBLOCK CACHEMEMORY::get_read_block(unsigned int addr, bool is_write_request){
    CACHEBLOCK cb;
    int tag_lsb = this->block_offset_bits_length + this->index_bits_length;
    cb.tag = this->get_masked_data(addr, this->tag_mask, tag_lsb);
    cb.block_offset = addr;
    cb.counter = 0;
    cb.is_dirty = is_write_request;
    cb.is_valid = true;

    return cb;
}


void CACHEMEMORY::insert_block_and_update_lru(int row, int hit, CACHEBLOCK& new_block, bool update_dirty_bit){

    int hit_block_counter = this->cache[row][hit].counter;

    for(int j=0;j<this->cache[row].size();j++){
        if(this->cache[row][j].is_valid){
            if(j == hit){
                this->cache[row][j].block_offset = new_block.block_offset;
                this->cache[row][j].tag = new_block.tag;
                this->cache[row][j].is_valid = true;
                if(update_dirty_bit)
                    this->cache[row][j].is_dirty = new_block.is_dirty;
                this->cache[row][j].counter = 0;
            }else{
                //Update the block with the new block details
                int val = this->cache[row][j].counter;
                if(val < hit_block_counter)
                    this->cache[row][j].counter++;
            }
        }
    }

}

void CACHEMEMORY::insert_block_replace_invalid_block_and_update_lru(int row, CACHEBLOCK& new_block, bool update_dirty_bit){

    int idx = 0;

    for(CACHEBLOCK block : this->cache[row]){
        if(!block.is_valid){
            break;
        }
        idx++;
    }

    for(int j=0;j<this->cache[row].size();j++){
        if(this->cache[row][j].is_valid && j != idx){
            //Update the block with the new block details
            this->cache[row][j].counter++;
        }
    }

    //Update the block with the new block details
    this->cache[row][idx].block_offset = new_block.block_offset;
    this->cache[row][idx].tag = new_block.tag;
    this->cache[row][idx].is_valid = true;
    if(update_dirty_bit)
        this->cache[row][idx].is_dirty = new_block.is_dirty;
    this->cache[row][idx].counter = 0;

}

int CACHEMEMORY::search_block_by_tag(int row, int tag){
    int hit_block_idx = -1;
    int idx = 0;

    for(CACHEBLOCK block : this->cache.at(row)){
        if(block.is_valid && block.tag == tag){
            return idx;
        }
        idx++;
    }
    return -1;
}

bool CACHEMEMORY::is_cache_full(int row){
    int idx = 0;

    for(CACHEBLOCK block : this->cache[row]){
        if(block.is_valid){
            idx++;
        }
    }
    return idx == this->cache[row].size();
}

CACHEBLOCK CACHEMEMORY::get_lru_block(int row){
    int idx = 0;
    int assoc = this->cache[row].size() - 1;
    CACHEBLOCK evict;
    for(CACHEBLOCK block : this->cache[row]){
        if(block.is_valid && block.counter == assoc){
            evict = block;
            break;
        }
        idx++;
    }

    this->cache[row][idx].is_valid = false;

    return evict;
}

bool CACHEMEMORY::read_request(unsigned int addr){
    // Get the cache line from addr and index mask
    int idx = get_masked_data(addr, this->index_mask,this->block_offset_bits_length);

    //Debug
    // if(idx == 1)
    //     cout<<hex<<"Addr with index 1: "<<"r "<<addr<<endl;

    // Search block from the cacheline
    CACHEBLOCK requested_block = this->get_read_block(addr, false);
    int block_idx = search_block_by_tag(idx, requested_block.tag);

    if(block_idx == -1){
        // Read Miss
        // fetch from next level 
        // insert new block
        // update lru
        // return false

        if(is_cache_full(idx)){
            // line is full --> evict block
            CACHEBLOCK evicted_block = get_lru_block(idx);
            if(this->next_mem != nullptr && evicted_block.is_dirty){ 
                unsigned int write_back_addr = evicted_block.block_offset;
                this->next_mem->write_request(write_back_addr);
            }
        }
        // fetch from next level
        if(this->next_mem != nullptr){
            this->next_mem->read_request(addr);
        }

        // insert new block
        // update lru
        insert_block_replace_invalid_block_and_update_lru(idx, requested_block, true);
        return false; 
    }else{
        // Read Hit --> return true and update lru counter
        insert_block_and_update_lru(idx, block_idx, requested_block, false);
        return true;
    }
    
}

bool CACHEMEMORY::write_request(unsigned int addr){
        // Get the cache line from addr and index mask
    int idx = get_masked_data(addr, this->index_mask,this->block_offset_bits_length);

    //Debug
    // if(idx == 1)
    //     cout<<"Addr with index 1: "<<"w "<<hex<<addr<<endl;

    // Search block from the cacheline
    CACHEBLOCK requested_block = this->get_read_block(addr, true);
    int block_idx = search_block_by_tag(idx, requested_block.tag);

    if(block_idx == -1){
        // Read Miss
        // fetch from next level 
        // insert new block
        // update lru
        // return false

        if(is_cache_full(idx)){
            // line is full --> evict block
            CACHEBLOCK evicted_block = get_lru_block(idx);
            if(this->next_mem != nullptr && evicted_block.is_dirty){ 
                unsigned int write_back_addr = evicted_block.block_offset;
                this->next_mem->write_request(write_back_addr);
            }
        }
        // fetch from next level
        if(this->next_mem != nullptr){
            this->next_mem->read_request(addr);
        }

        // insert new block
        // update lru
        insert_block_replace_invalid_block_and_update_lru(idx, requested_block, true);
        return false; 
    }else{
        // Read Hit --> return true and update lru counter
        insert_block_and_update_lru(idx, block_idx, requested_block, true);
        return true;
    }
}
