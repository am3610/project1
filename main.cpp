#include <iostream>
#include "library.h"

using namespace std;

int main(int argc, char** argv){
	if(argc < 3){
		cout << "usage: ./lib <resource file> <input file>" << endl;
	}
	else{
		library lib(argv);
	}

	return 0;
}
