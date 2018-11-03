#include "resource.h"

using namespace std;

resource::resource(string title){
	this->title = title;
}

void resource::setBorrowDate(string borrowDate){
	date tmp(borrowDate);
	this->borrowDate = tmp;	
}

string resource::getBorrowDate(){
	return borrowDate.getDate();
}

string resource::getTitle(){
	return title;
}

book::book(string title) : resource(title){
	u_borrower = NULL;
}

void book::setUndergraduate(undergraduate *ug){
	u_borrower = ug;
}

void book::freeUndergraduate(){
	u_borrower = NULL;
}

int book::isOccupied(){
	int ret = 0;
	if(u_borrower != NULL){
		ret = 1;
	}

	return ret;
}

bool book::isLate(const string &retDate, const string &mType,
		string &tmp){
	int ret = false;
	date r(retDate);
	date d(this->getBorrowDate());
	int cond = r - d;

	if(!mType.compare("Undergraduate")){
		if(cond > 13){
			date p;
			p = r + (cond - 13);
			tmp = p.getDate();
			u_borrower->setPaneltyDate(p);
			ret = true;
		}
	}

	return ret;
}
