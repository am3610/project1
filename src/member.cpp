#include "member.h"

using namespace std;

member::member(int id, string name){
	this->id = id;
	this->name = name;
}

string member::getName(){
	return this->name;
}

void member::setLimit(int limit){
	this->limit = limit;
}

undergraduate::undergraduate(int id, string name) : member(id, name){
	this->setLimit(1);
}
