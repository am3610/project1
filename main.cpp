#include <iostream>
#include "library.h"

using namespace std;

int main(int argc, char** argv){
	if(argc < 4){
		cout << "usage: ./lib <resource.dat> <input.dat> <space.dat>" << endl;
	}
	else{
		library lib(argv);
	}

	return 0;
}
