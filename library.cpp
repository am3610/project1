#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include "library.h"

using namespace std;

library::library(char** argv){
	count = 0;

	ifstream ifs(argv[1]);
	string str;
	string t, n;

	getline(ifs, str);				// attribute(ignore)
	getline(ifs, str);
	while(!ifs.eof()){
		stringstream ss(str);
		ss >> t >> n;
		if(!t.compare("Book")){
			books.insert(pair<string, book*>(n, new book(n)));
		}
		getline(ifs, str);
	}

	ifs.close();

	ofstream ofs("output.dat");
	ofs << "Op_#\tReturn_code\tDescription" << endl;

	///////////////////////////////////////////////////////////////////////

	ifstream rs(argv[2]);
	ifstream ss(argv[3]);

	queue<string> res;

	getline(rs, str);
	getline(rs, str);
	while(!rs.eof()){
		res.push(str);
		getline(rs, str);
	}

	queue<string> spc;

	getline(ss, str);
	getline(ss, str);
	while(!ss.eof()){
		spc.push(str);
		getline(ss, str);
	}

	while(!res.empty() && !spc.empty()){
		stringstream rss(res.front());
		stringstream sss(spc.front());
		string r, s;
		rss >> r;
		sss >> s;
		date rd(r);
		date sd(s);

		if(sd - rd < 0){
			spcManager(spc.front(), ofs);
			spc.pop();
		}
		else{
			resManager(res.front(), ofs);
			res.pop();
		}
	}

	while(!spc.empty()){
		spcManager(spc.front(), ofs);
		spc.pop();
	}

	while(!res.empty()){
		resManager(res.front(), ofs);
		res.pop();
	}

	rs.close();
	ss.close();
	ofs.close();
}

static void readSet(struct opset &set, string str){
	stringstream ss(str);
	ss >> set.d >> set.r_t >> set.r_n >> set.o >> set.m_t >> set.m_n; 
}

void library::resManager(string str, ofstream &ofs){

	struct opset set;

	readSet(set, str);  

	count += 1;

	mem_add(set.m_t, set.m_n);
	if(!(set.o).compare("B") && check_1(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_2(set, count, ofs)) {}
	else if(!(set.o).compare("R") && check_3(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_4(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_5(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_6(set, count, ofs)) {}
	else if(!(set.o).compare("R") && check_7(set, count, ofs)) {}
	else{
		if(!(set.o).compare("B")){
			borrowRes(set);
		}
		else{
			returnRes(set);
		}

		ofs << count << "\t0\tSuccess." << endl;
	}
}

void library::spcManager(string str, ofstream &ofs){
	ofs << str << endl;
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
