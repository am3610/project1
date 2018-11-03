#include "member.h"
#include <iostream>

using namespace std;

member::member(string name){
	date d("00/00/00");
	this->paneltyDate = d;
	this->name = name;
}

void member::setPaneltyDate(date paneltyDate){
	int compare = paneltyDate - (this->paneltyDate);
	if(compare > 0){
		this->paneltyDate = paneltyDate;
	}
}

undergraduate::undergraduate(string name) : member(name){
	limit = 1;
}

void undergraduate::addInfo(string type, string title){
	info.push_back(new struct resInfo(type, title));
}

void undergraduate::eraseInfo(string type, string title){
	for(auto i = info.begin(); i != info.end(); i++){
		if(!type.compare((*i)->type) && !title.compare((*i)->title)){
			delete (*i);
			info.erase(i);
			break;
		}
	}
}

bool undergraduate::isExist(string type, string title){
	bool ret = false;
	for(auto i : info){
		if(!type.compare(i->type) && !title.compare(i->title)){
			ret = true;
			break;
		}
	}
	return ret;
}

bool undergraduate::isOver(){
	bool ret;
	if(info.size() < limit) ret = false;
	else ret = true;
	return ret;
}

int undergraduate::getLimit(){
	return limit;
}

bool undergraduate::isRestricted(const string &borrDate, string &tmp){
	bool ret = false;
	date b(borrDate);
	int compare = paneltyDate - b;
	if(compare >= 0){
		tmp = paneltyDate.getDate();
		ret = true;
	}

	return ret;
}
