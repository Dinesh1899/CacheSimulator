#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <math.h>
#include <vector>

using namespace std;

struct CACHEBLOCK
{
    /* data */
    unsigned int tag;
    unsigned int block_offset;
    bool is_valid;
    bool is_dirty;
    int counter; // LRU Counter

    bool operator==(const CACHEBLOCK& cb) const
    {
        return (this->tag == cb.tag);
    }

};


class CACHEMEMORY
{
private:
    /* data */
    // Map index -> Set of CACHEBLOCKS
    vector<vector<CACHEBLOCK>> cache;
    set<CACHEBLOCK>* vc;
    CACHEMEMORY* next_mem;

    unsigned int index_mask;
    unsigned int tag_mask;
    unsigned int block_offset_mask;

    unsigned int tag_bits_length;
    unsigned int index_bits_length;
    unsigned int block_offset_bits_length;


public:
    static const int ADDR_LENGTH = 32;
    CACHEMEMORY(/* args */);
    ~CACHEMEMORY();
    CACHEMEMORY(int size, int blocksize, int assoc, int vc_blocks);
    
    void set_tag_mask(int tag_length);
    void set_index_mask(int index_length, int block_offset_length);
    void set_block_offset_mask(int block_offset_length);
    void set_masks(int blocksize, int num_sets);
    int get_mask(int right, int left);
    int get_masked_data(int addr, int mask, int right);
    CACHEBLOCK get_read_block(int addr);
    void show();

    bool read_request(int addr);
};

CACHEMEMORY::CACHEMEMORY(){}

CACHEMEMORY::~CACHEMEMORY(){}

CACHEMEMORY::CACHEMEMORY(int size, int blocksize, int assoc, int vc_blocks){
    int num_sets = size/(blocksize * assoc);
    //this->cache.resize(num_sets);
    set_masks(blocksize, num_sets);
    
    for(int i=0;i<num_sets;i++){
        vector<CACHEBLOCK> cache_set;
        for(int j=0;j<assoc;j++){
            CACHEBLOCK block;
            block.tag = j;
            block.is_valid = false;
            
            cache_set.push_back(block);
        }
        this->cache.push_back(cache_set);
    } 

}

void CACHEMEMORY::show(){
   
    for(int i = 0; i < this->cache.size(); i++){
        cout<<"set "<<i<<" ";
        for(CACHEBLOCK block : this->cache[i]){
            cout<<block.tag<<" "<<block.is_valid<<" "<<block.is_dirty<<" ";
        }
        cout<<endl;
    }

}

void CACHEMEMORY::set_masks(int blocksize, int num_sets){

    int block_offset_length = log2(blocksize);
    int index_length = log2(num_sets);
    int tag_length = ADDR_LENGTH - index_length - block_offset_length;

    this->tag_bits_length = tag_length;
    this->index_bits_length = index_length;
    this->block_offset_bits_length = block_offset_length;

    set_block_offset_mask(block_offset_length);
    set_index_mask(index_length, block_offset_length);
    set_tag_mask(tag_length);

}

void CACHEMEMORY::set_block_offset_mask(int block_offset_length){
    this->block_offset_mask = get_mask(0, block_offset_length-1);
}

void CACHEMEMORY::set_index_mask(int index_length, int block_offset_length){
    this->index_mask = get_mask(block_offset_length, block_offset_length+index_length-1);
}

void CACHEMEMORY::set_tag_mask(int tag_length){
    this->tag_mask = get_mask(ADDR_LENGTH-tag_length,ADDR_LENGTH);
}

int CACHEMEMORY::get_mask(int right, int left){
    return ((1 << (left - right + 1)) - 1) << right;
}

int test_mask(int right, int left){
    return ((1 << (left - right + 1)) - 1) << right;
}

int main(){

    int num = 0xeff5;
    int mask = test_mask(1,2);
    int res = (num & mask) >> 1;
    cout<<"Mask is: "<<mask<<endl;
    cout<<"Result is: "<<res<<endl;
    return 0;
}