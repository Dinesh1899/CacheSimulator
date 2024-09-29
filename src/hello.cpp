#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//#include "request_handlers.cpp"
#include "victim_request_handlers.cpp"
#include <vector>
using namespace std;



unsigned int L1_SIZE;            //L1 cache size in Bytes
unsigned int L1_ASSOC;           //L1 set-associativity
unsigned int L1_BLOCKSIZE;       //L1 block size in Bytes

unsigned int VC_NUM_BLOCKS;		 //Number of blocks in the Victim Cache (0 if no VC)

unsigned int L2_SIZE;            //L2 cache size in bytes. (0 if no L2)
unsigned int L2_ASSOC;           //L2 set-associativity

char *tracefile;                 //Character string specifying the full name of trace file


int extractBits(int number, int p, int q) {
    int mask = ((1 << (q - p + 1)) - 1) << p;
    return (number & mask) >> p;
}


// 1110 -> e
// 1111 -> f

int main(int argc, char* argv[]){

    L1_SIZE = static_cast<unsigned int>(strtoul(argv[1], 0, 10));
	L1_ASSOC = static_cast<unsigned int>(strtoul(argv[2], 0, 10));
    L1_BLOCKSIZE = static_cast<unsigned int>(strtoul(argv[3], 0, 10));
	VC_NUM_BLOCKS = static_cast<unsigned int>(strtoul(argv[4], 0, 10));
    L2_SIZE = static_cast<unsigned int>(strtoul(argv[5], 0, 10));
    L2_ASSOC = static_cast<unsigned int>(strtoul(argv[6], 0, 10));
    tracefile = argv[7];

    cout << "===== Simulator configuration ====="<<endl;
	cout << "  L1_SIZE:\t\t" << L1_SIZE<<endl;
    cout << "  L1_ASSOC:\t\t" << L1_ASSOC<<endl;	
    cout << "  L1_BLOCKSIZE:\t\t" << L1_BLOCKSIZE<<endl;
	cout << "  VC_NUM_BLOCKS:\t" << VC_NUM_BLOCKS<<endl;
    cout << "  L2_SIZE:\t\t" << L2_SIZE<<endl;
    cout << "  L2_ASSOC:\t\t" << L2_ASSOC<<endl;
    cout << "  trace_file:\t\t" << tracefile << endl;
	cout<<endl;



	if(VC_NUM_BLOCKS == 0){
		CACHEMEMORY *L1 = new CACHEMEMORY(L1_SIZE, L1_BLOCKSIZE, L1_ASSOC);
		CACHEMEMORY *L2 = nullptr;

		if(L2_SIZE > 0){
			L2 = new CACHEMEMORY(L2_SIZE, L1_BLOCKSIZE, L2_ASSOC);
			// Append L1 to L2
			L1->append(L2);
		}

		// Open the trace file
		string trace_file_dir = "samples/traces/";
		string trace_file_path = trace_file_dir + tracefile;
		ifstream file(trace_file_path);
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

		// Close the file
		file.close();
		L1->sort_each_set();
		cout<<"===== L1 contents ====="<<endl;
		L1->show();
		if(L2_SIZE > 0){
			L2->sort_each_set();
			cout<<"===== L2 contents ====="<<endl;
			L2->show();
		}
	}else{
		L1VC *L1 = new L1VC(L1_SIZE, L1_BLOCKSIZE, L1_ASSOC, VC_NUM_BLOCKS);
		CACHEMEMORY *L2 = nullptr;

		if(L2_SIZE > 0){
			L2 = new CACHEMEMORY(L2_SIZE, L1_BLOCKSIZE, L2_ASSOC);
			// Append L1 to L2
			L1->append(L2);
		}

		// Open the trace file
		string trace_file_dir = "samples/traces/";
		string trace_file_path = trace_file_dir + tracefile;
		ifstream file(trace_file_path);
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

		// Close the file
		file.close();
		L1->sort_each_set();
		cout<<"===== L1 contents ====="<<endl;
		L1->show();
		if(L2_SIZE > 0){
			L2->sort_each_set();
			cout<<"===== L2 contents ====="<<endl;
			L2->show();
		}
	}





	return 0;
}

