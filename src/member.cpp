#include "member.h"

using namespace std;

member::member(string name){
	this->name = name;
}

string member::getName(){
	return this->name;
}

undergraduate::undergraduate(string name) : member(name){}
