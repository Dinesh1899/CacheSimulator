#include <iostream>
#include "parse.h"

using namespace std;

int main(){

    unsigned int size = 1024;
	unsigned int assoc = 2;
	unsigned int block_size = 16;

	float access_time = 0;
	float energy = 0;
	float area = 0;

	int cacti = get_cacti_results(size, block_size, assoc, &access_time, &energy, &area);

	if(cacti > 0){
		cout<<"Cacti Failed"<<endl;
	}

	cout<<"Access Time: "<<access_time<<" Energy: "<<energy<<" area "<<area<<endl;

	return 0;

}