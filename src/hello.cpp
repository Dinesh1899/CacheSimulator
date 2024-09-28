#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
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

	CACHEMEMORY *L1 = new CACHEMEMORY(512, 16, 2, 0);
	CACHEMEMORY *L2 = new CACHEMEMORY(8192, 256, 2, 0);
	
	// Append L1 to L2
	L1->append(L2);

	vector<unsigned int> traces = {
		0xa111100f, 0xb111110f, 0xc111121f, 0xd111131f, 0xe111140f
	};


    // Open the trace file
    ifstream file("samples/traces/tracefile.txt");
    if (!file.is_open()) {
        cerr << "Unable to open file!" << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        // Check if the line starts with 'r' or 'w'
        if (line[0] == 'r' || line[0] == 'w') {
            char type = line[0];
            string hexValue = line.substr(1); // Extract the substring after 'r' or 'w'

            // Convert the hexadecimal string to an integer (optional)
            unsigned int addr;
            stringstream ss;
            ss << hex << hexValue;
            ss >> addr;

			if(type == 'r'){
				L1->read_request(addr);
			}else if(type == 'w'){
				L1->write_request(addr);
			}else{
				cerr<<"Invalid Request Type"<<endl;
			}
        }
    }

	// CACHEMEMORY *L1 = new CACHEMEMORY(512, 16, 2, 0);

	// for(unsigned int addr : traces){
	// 	L1->read_request(addr);
	// }
	cout<<"=====================L1 Contents===================="<<endl;
	L1->show();
	cout<<"=====================L2 Contents===================="<<endl;
	L2->show();
	cout<<"Run Cache Simulator"<<endl<<"Done"<<endl;

	// Close the file
	file.close();
	return 0;
}

