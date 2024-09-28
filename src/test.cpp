#include <iostream>
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

};

int main(){    
    vector<CACHEBLOCK> v4;
    v4.reserve(10);

    CACHEBLOCK b;
    b.is_dirty = true;
    b.is_valid = true;

    v4.push_back(b);

    cout<<"Printing V4"<<endl;
    for(CACHEBLOCK& block : v4){
        cout<<hex<<block.tag<<" "<<block.is_valid<<" "<<block.is_dirty<<" "<<" "<<block.counter<<" "<<block.block_offset<<endl;
    }

    return 0;
}

