#include <iostream>
#include <string>
#include "member.h"

using namespace std;

int main(void){
	undergraduate *m = new undergraduate("hyeonho");
	cout << m->getName() << endl;
	return 0;
}
