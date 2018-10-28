#include <iostream>
#include <string>
#include "date.h"

using namespace std;

int main(void){
	/*undergraduate *m = new undergraduate(1, "hyeonho");
	cout << m->getName() << endl;*/
	date d("12/12/30");
	//date c;
	//c = d + 12;
	cout << d.getYear() << " " << d.getMonth() << " " << d.getDay() << endl;
	//cout << c - d << endl;
	return 0;
}
