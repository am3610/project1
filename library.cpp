#include <fstream>
#include <iostream>
#include "library.h"

using namespace std;

library::library(char* res){
	ifstream ifs(res, ifstream::in);
	string input;
	int route = 0;
	
	ifs >> input;					// attribute (ignore)
	ifs >> input;

	ifs >> input;					// start a real data
	while(!ifs.eof()){
		if(route == 0){
			if(!input.compare("Book")){
				route = 1;
			}
		}
		else{
			switch(route){
				case 1:
					books.insert(pair<string, book*>(input, new book(input)));
					break;
				default:
					break;
			}
			route = 0;
		}
		ifs >> input;
	}
	ifs.close();
}

static void readSet(struct opset &set, ifstream &ifs){
	ifs >> set.d >> set.r_t >> set.r_n >> set.o >> set.m_t >> set.m_n; 
}

void library::execute(char *inp){
	ifstream ifs(inp, ifstream::in);
	ofstream ofs("output.dat");
	struct opset set;

	readSet(set, ifs);  // attribute (ignore) 

	readSet(set, ifs);

	int count = 0;
	ofs << "Op_#\tReturn_code\tDescription" << endl;
	while(!ifs.eof()){
		count += 1;
		mem_add(set.m_t, set.m_n);
		if(!(set.o).compare("B") && check_1(set, count, ofs)) {readSet(set, ifs); continue;}
		else if(!(set.o).compare("B") && check_2(set, count, ofs)) {readSet(set, ifs); continue;}
		else if(!(set.o).compare("R") && check_3(set, count, ofs)) {readSet(set, ifs); continue;}
		else if(!(set.o).compare("B") && check_4(set, count, ofs)) {readSet(set, ifs); continue;}
		else if(!(set.o).compare("B") && check_5(set, count, ofs)) {readSet(set, ifs); continue;}
		else if(!(set.o).compare("B") && check_6(set, count, ofs)) {readSet(set, ifs); continue;}
		else if(!(set.o).compare("R") && check_7(set, count, ofs)) {readSet(set, ifs); continue;}
		else{
			if(!(set.o).compare("B")){
				borrowRes(set);
			}
			else{
				returnRes(set);
			}
			ofs << count << "\t0\tSuccess." << endl;
			readSet(set, ifs);
		}
	}
}

void library::mem_add(string type, string name){
	if(!type.compare("Undergraduate")){
		map<string, undergraduate*>::iterator it = undergraduates.find(name);
		if(it == undergraduates.end()){
			undergraduates.insert(pair<string, undergraduate*>
					(name, new undergraduate(name)));	
		}
	}
}

bool library::check_1(struct opset op, const int count, ofstream &ofs){
	bool ret = false;

	if(!(op.r_t).compare("Book")){
		map<string, book*>::iterator it = books.find(op.r_n);
		if(it == books.end()){
			ofs << count << "\t1\tNon exist resource." << endl;
			ret = true;
		}
	}

	return ret;
}

bool library::check_2(struct opset op, const int count, ofstream &ofs){
	bool ret = false;
	if(!(op.m_t).compare("Undergraduate")){
		map<string, undergraduate*>::iterator it = undergraduates.find(op.m_n);
		if((it->second)->isOver()){
			ofs << count << "\t2\tExceeds your possible number of borrow. Possible # of borrows: "
				<< (it->second)->getLimit() << endl; 
			ret = true;
		}
	}
	
	return ret;
}

bool library::check_3(struct opset op, const int count, ofstream &ofs){
	bool ret = false;
	if(!(op.m_t).compare("Undergraduate")){
		map<string, undergraduate*>::iterator it = undergraduates.find(op.m_n);
		if(!((it->second)->isExist(op.r_t, op.r_n))){
			ofs << count << "\t3\tYou did not borrow this book." << endl;
			ret = true;
		}
	}
	return ret;
}

bool library::check_4(struct opset op, const int count, ofstream &ofs){
	bool ret = false;
	if(!(op.m_t).compare("Undergraduate")){
		map<string, undergraduate*>::iterator it = undergraduates.find(op.m_n);
		if((it->second)->isExist(op.r_t, op.r_n)){
			if(!(op.r_t).compare("Book")){
				map<string, book*>::iterator bt = books.find(op.r_n);
				ofs << count << "\t4\tYou already borrowed this book at" 
					<< (bt->second)->getBorrowDate() << endl;
				ret = true;
			}
		}
	}
	return ret;
}

bool library::check_5(struct opset op, const int count, ofstream &ofs){
	bool ret = false;
	if(!(op.r_t).compare("Book")){
		map<string, book*>::iterator it = books.find(op.r_n);
		if((it->second)->isOccupied() == 1){
			date tmp((it->second)->getBorrowDate());
			tmp = tmp + 13;
			ofs << count << 
				"\t5\tOther member already borrowed this book. This book will be returned at " 
				<< tmp.getDate() << endl;
			ret = true;
		}
	}
	return ret;
}

bool library::check_6(struct opset op, const int count, ofstream &ofs){
	bool ret = false;
	if(!(op.m_t).compare("Undergraduate")){
		map<string, undergraduate*>::iterator it = undergraduates.find(op.m_n);
		string tmp;
		if((it->second)->isRestricted(op.d, tmp)){
			ofs << count << "\t6\tRestricted member until " 
				<< tmp << endl;
			ret = true;
		}
	}

	return ret;
}

bool library::check_7(struct opset op, const int count, ofstream &ofs){
	bool ret = false;

	if(!(op.r_t).compare("Book")){
		map<string, book*>::iterator it = books.find(op.r_n);
		string tmp;
		if((it->second)->isLate(op.d, op.m_t, tmp)){
			returnRes(op);
			ofs << count << 
				"\t7\tDelayed return. You'll be restricted until " 
				<< tmp << endl;
			ret = true;
		}
	}
	return ret;
}

void library::borrowRes(struct opset op){
	if(!(op.m_t).compare("Undergraduate")){
		map<string, undergraduate*>::iterator mt = undergraduates.find(op.m_n);
		(mt->second)->addInfo(op.r_t, op.r_n);

		if(!(op.r_t).compare("Book")){
			map<string, book*>::iterator rt = books.find(op.r_n);
			(rt->second)->setUndergraduate(mt->second);
			(rt->second)->setBorrowDate(op.d);
		}
	}
}

void library::returnRes(struct opset op){
	if(!(op.m_t).compare("Undergraduate")){
		map<string, undergraduate*>::iterator mt = undergraduates.find(op.m_n);
		(mt->second)->eraseInfo(op.r_t, op.r_n);

		if(!(op.r_t).compare("Book")){
			map<string, book*>::iterator rt = books.find(op.r_n);
			(rt->second)->freeUndergraduate();
		}
	}
}
