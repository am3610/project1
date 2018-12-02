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

void resource::setUndergraduate(undergraduate *ug){
	u_borrower = ug;
}

void resource::freeUndergraduate(){
	u_borrower = NULL;
}

bool resource::isOccupied(){
	int ret = false;
	if(u_borrower != NULL){
		ret = true;
	}

	return ret;
}

bool resource::isLate(const string &retDate, const string &mType,
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

book::book(string title) : resource(title){
	u_borrower = NULL;
}

magazine::magazine(string title) : resource(title){
	u_borrower = NULL;
}

ebook::ebook(string title, int size) : resource(title){
	this->size = size;
}

int ebook::getSize(){
	return size;
}

