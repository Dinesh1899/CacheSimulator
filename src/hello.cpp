#include <iostream>
#include "cache_mem.cpp"
#include <vector>
using namespace std;

int extractBits(int number, int p, int q) {
    int mask = ((1 << (q - p + 1)) - 1) << p;
    return (number & mask) >> p;
}


int main(){
	//long num = stoi("a",0,16);
	//cout<<"Num is: "<<num<<endl;
	//cout<<"Size of int is: "<<sizeof(int)<<endl;

	//cout<<extractBits(6, 0, 0)<<endl; // 6 -> 110 

	CACHEMEMORY *L1 = new CACHEMEMORY(64, 16, 2, 0);

	L1->show();
	// vector<int> vec;
	// vec.push_back(52);
	// vec.push_back(72);
	// cout<<"First: "<<vec.at(1)<<endl;
	cout<<"Run Cache Simulator"<<endl<<"Done"<<endl;
	return 0;
}
