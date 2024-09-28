#include <iostream>
#include "request_handlers.cpp"
#include <vector>
using namespace std;

int extractBits(int number, int p, int q) {
    int mask = ((1 << (q - p + 1)) - 1) << p;
    return (number & mask) >> p;
}


// 1110 -> e
// 1111 -> f

int main(){
	//long num = stoi("a",0,16);
	//cout<<"Num is: "<<num<<endl;
	//cout<<"Size of int is: "<<sizeof(int)<<endl;

	//cout<<extractBits(6, 0, 0)<<endl; // 6 -> 110 

	vector<unsigned int> traces = {
		0xa111110f, 0xb111110f, 0xc111111f, 0xd111111f, 0xe111110f
	};


	CACHEMEMORY *L1 = new CACHEMEMORY(512, 16, 2, 0);

	for(unsigned int addr : traces){
		L1->read_request(addr);
	}

	L1->show();
	cout<<"Run Cache Simulator"<<endl<<"Done"<<endl;
	return 0;
}

