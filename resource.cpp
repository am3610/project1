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

void resource::setGraduate(graduate *gr){
	g_borrower = gr;
}

void resource::setFaculty(faculty *fc){
	f_borrower = fc;
}

void resource::freeUndergraduate(){
	u_borrower = NULL;
}

void resource::freeGraduate(){
	g_borrower = NULL;
}

void resource::freeFaculty(){
	f_borrower = NULL;
}

int resource::isOccupied(){
	int ret = 0;
	if(u_borrower != NULL){
		ret = 1;
	}else if(g_borrower != NULL){
		ret = 2;
	}else if(f_borrower != NULL){
		ret = 3;
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
		if(cond >= 14){
			date p;
			p = r + (cond - 13);
			tmp = p.getDate();
			u_borrower->setPaneltyDate(p);
			ret = true;
		}
	}else if(!mType.compare("Graduate")){
		if(cond >= 30){
			date p;
			p = r + (cond - 29);
			tmp = p.getDate();
			g_borrower->setPaneltyDate(p);
			ret = true;
		}
	}else if(!mType.compare("Faculty")){
		if(cond >= 30){
			date p;
			p = r + (cond - 29);
			tmp = p.getDate();
			f_borrower->setPaneltyDate(p);
			ret = true;
		}
	}

	return ret;
}

book::book(string title) : resource(title){
	u_borrower = NULL;
	g_borrower = NULL;
	f_borrower = NULL;
}

magazine::magazine(string title) : resource(title){
	u_borrower = NULL;
	g_borrower = NULL;
	f_borrower = NULL;
}

ebook::ebook(string title, int size) : resource(title){
	this->size = size;
}

int ebook::getSize(){
	return size;
}

