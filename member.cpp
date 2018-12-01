#include "member.h"
#include <iostream>

using namespace std;

member::member(string name){
	date d("00/00/00");
	this->paneltyDate = d;
	this->name = name;
	this->studyRoom_no = 0;
	this->seatFloor = 0;
}

void member::setPaneltyDate(date paneltyDate){
	int compare = paneltyDate - (this->paneltyDate);
	if(compare > 0){
		this->paneltyDate = paneltyDate;
	}
}

void member::addInfo(string type, string title){
	info.push_back(new struct resInfo(type, title));
}

void member::eraseInfo(string type, string title){
	for(auto i = info.begin(); i != info.end(); i++){
		if(!type.compare((*i)->type) && !title.compare((*i)->title)){
			delete (*i);
			info.erase(i);
			break;
		}
	}
}

bool member::isExist(string type, string title){
	bool ret = false;
	for(auto i : info){
		if(!type.compare(i->type) && !title.compare(i->title)){
			ret = true;
			break;
		}
	}
	return ret;
}

bool member::isOver(){
	bool ret;
	int cnt = 0;

	for(auto i : info){
		if((i->type).compare("E-book")){
			cnt += 1;
		}
	}

	if(cnt < limit) ret = false;
	else ret = true;
	return ret;
}

int member::getLimit(){
	return limit;
}

bool member::isRestricted(const string &borrDate, string &tmp){
	bool ret = false;
	date b(borrDate);
	int compare = paneltyDate - b;
	if(compare >= 0){
		tmp = paneltyDate.getDate();
		ret = true;
	}

	return ret;
}

void member::borrowStudyRoom(int no){
	this->studyRoom_no = no;
}

void member::returnStudyRoom(){
	this->studyRoom_no = 0;
}

void member::borrowSeat(int floor){
	this->seatFloor = floor;
}

void member::returnSeat(){
	this->seatFloor = 0;
}

int member::retStudyRoom(){
	return studyRoom_no;
}

int member::retSeat(){
	return seatFloor;
}

void member::memSub(int size){
	memory -= size;
}

void member::memAdd(int size){
	memory += size;
}

bool member::memOver(int size){
	bool ret = false;
	if(memory - size < 0){
		ret = true;
	}
	return ret;
}

undergraduate::undergraduate(string name) : member(name){
	limit = 1;
	memory = 100;
}

