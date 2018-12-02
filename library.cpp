#include <iostream>
#include <fstream>
#include <sstream>
#include "library.h"

using namespace std;

static pair<string, int> parseTitle(string title){
	string t;
	string n;
	bool f = false;
	for(auto s : title){
		if(!f){
			if(s == '['){
				f = true;
				continue;
			}
			
			t.push_back(s);

		}else{
			if(s == ']'){
				break;
			}

			n.push_back(s);
		}
	}

	return make_pair(t, stoi(n));
}

library::library(char* res){
	ifstream ifs(res);
	string str;
	string t, n;

	getline(ifs, str);				// attribute(ignore)
	getline(ifs, str);
	while(!ifs.eof()){
		stringstream ss(str);
		ss >> t >> n;
		if(!t.compare("Book")){
			books.insert(pair<string, book*>(n, new book(n)));
		}else if(!t.compare("Magazine")){
			magazines.insert(n);
		}else if(!t.compare("E-book")){
			pair<string, int> p = parseTitle(n);
			ebooks.insert(pair<string, ebook*>(p.first, new ebook(p.first, p.second)));
		}
		getline(ifs, str);
	}

	ifs.close();
}

static void readSet(struct opset &set, ifstream &ifs){
	string str;
	getline(ifs, str);
	stringstream ss(str);
	ss >> set.d >> set.r_t >> set.r_n >> set.o >> set.m_t >> set.m_n; 
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
		else if(!(set.o).compare("B") && check_15(set, count, ofs)) {readSet(set, ifs); continue;}
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
	}/*else if(!(op.r_t).compare("Magazine")){
		set<string>::iterator it = megazines.find(op.r_n);
		if(it == magazines.end()){
			ofs << count << "\t1\tNon exist resource." << endl;
			ret = true;
		}
	}*/else if(!(op.r_t).compare("E-book")){
		map<string, ebook*>::iterator it = ebooks.find(op.r_n);
		if(it == ebooks.end()){
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

		string tmp;
		if((it->second)->isExist(op.r_t, op.r_n, tmp)){
			ofs << count << "\t4\tYou already borrowed this book at " 
				<< tmp << endl;
			ret = true;
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

bool library::check_15(struct opset op, const int count, ofstream &ofs){
	bool ret = false;

	if(!(op.r_t).compare("E-book")){
		map<string, ebook*>::iterator rt = ebooks.find(op.r_n);
		if(!(op.m_t).compare("Undergraduate")){
			map<string, undergraduate*>::iterator it = undergraduates.find(op.m_n);

			if((it->second)->memOver((rt->second)->getSize())){
				ofs << count << "\t15\tExceeds your storage capacity." << endl;
				ret = true;
			}
		}
	}
	return ret;
}

void library::borrowRes(struct opset op){
	if(!(op.m_t).compare("Undergraduate")){
		map<string, undergraduate*>::iterator mt = undergraduates.find(op.m_n);
		(mt->second)->addInfo(op.r_t, op.r_n, op.d);

		if(!(op.r_t).compare("Book")){
			map<string, book*>::iterator rt = books.find(op.r_n);
			(rt->second)->setUndergraduate(mt->second);
			(rt->second)->setBorrowDate(op.d);
		}else if(!(op.r_t).compare("E-book")){
			map<string, ebook*>::iterator rt = ebooks.find(op.r_n);
			(mt->second)->memSub((rt->second)->getSize());
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
		}else if(!(op.r_t).compare("E-book")){
			map<string, ebook*>::iterator rt = ebooks.find(op.r_n);
			(mt->second)->memAdd((rt->second)->getSize());
		}
	}
}
