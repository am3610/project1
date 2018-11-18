#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include "library.h"

#define SR_SIZE			10
#define SSIZE			50

#define FREE			0
#define EMPTY			1
#define OCCUPIED		2

using namespace std;

static pair<string, string> parseTitle(string title){
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

	return make_pair(t, n);
}

library::library(char** argv){
	count = 0;

	for(int i = 0; i < SR_SIZE; i++){
		rooms.push_back(new studyRoom());
	}

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
		}else if(!t.compare("Magazine")){
			map<string, magazine*>* mag = new map<string, magazine*>();
			magazines.emplace(n, *mag);
		}else if(!t.compare("E-book")){
			pair<string, string> p = parseTitle(n);
			ebooks.insert(pair<string, ebook*>(p.first, new ebook(p.first, stoi(p.second))));
		}
		getline(ifs, str);
	}

	ifs.close();

	ofstream ofs("output.dat");
	ofs << "Op_#\tReturn_code\tDescription" << endl;

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

static void readResSet(struct opset &set, string str){
	stringstream ss(str);
	ss >> set.d >> set.r_t >> set.r_n >> set.o >> set.m_t >> set.m_n; 
}

static void readSpcSet(struct spset &set, string str){
	stringstream ss(str);
	ss >> set.d >> set.s_t >> set.s_n >> set.o >> set.m_t >> set.m_n >> set.n_m >> set.t; 
}

void library::resManager(string str, ofstream &ofs){
	struct opset set;

	readResSet(set, str);  

	count += 1;

	mem_add(set.m_t, set.m_n);
	if(check_1(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_2(set, count, ofs)) {}
	else if(!(set.o).compare("R") && check_3(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_4(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_5(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_6(set, count, ofs)) {}
	else if(!(set.o).compare("R") && check_7(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_15(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_16(set, count, ofs)) {}
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
	struct spset set;

	readSpcSet(set, str);  

	count += 1;

	mem_add(set.m_t, set.m_n);
	
	spcReset(set.d);
	if(check_8(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_9(set, count, ofs)) {}
	else if((set.o).compare("B") && check_10(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_11(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_12(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_13(set, count, ofs)) {}
	else if(!(set.o).compare("B") && check_14(set, count, ofs)) {}
	else{
		if(!(set.s_t).compare("StudyRoom")){
			if(!(set.o).compare("B")){
				if(!(set.m_t).compare("Undergraduate")){
					map<string, undergraduate*>::iterator it = undergraduates.find(set.m_n);
					(it->second)->borrowStudyRoom(stoi(set.s_n));
				}
				rooms.at(stoi(set.s_n) - 1)->borrowRoom(set.d, set.m_t, set.m_n, set.t);
			}
			else{
				if(!(set.m_t).compare("Undergraduate")){
					map<string, undergraduate*>::iterator it = undergraduates.find(set.m_n);
					(it->second)->returnStudyRoom();
				}
				rooms.at(stoi(set.s_n) - 1)->returnRoom();
			}
		}
		else{
			if(!(set.o).compare("B")){
				if(!(set.m_t).compare("Undergraduate")){
					map<string, undergraduate*>::iterator it = undergraduates.find(set.m_n);
					uf[(stoi(set.s_n) - 1)].insert(pair<undergraduate*, seat*>(it->second, new seat(set.d, set.m_t, set.m_n, set.t)));
					(it->second)->borrowSeat(stoi(set.s_n));
				}
			}
			else if(!(set.o).compare("E")){
				if(!(set.m_t).compare("Undergraduate")){
					map<string, undergraduate*>::iterator it = undergraduates.find(set.m_n);	
					uf[(stoi(set.s_n) - 1)].at(it->second)->empty(set.t);
				}
			}
			else if(!(set.o).compare("C")){
				if(!(set.m_t).compare("Undergraduate")){
					map<string, undergraduate*>::iterator it = undergraduates.find(set.m_n);	
					uf[(stoi(set.s_n) - 1)].at(it->second)->comeback();
				}
			}
			else{
				if(!(set.m_t).compare("Undergraduate")){
					map<string, undergraduate*>::iterator it = undergraduates.find(set.m_n);
					delete uf[(stoi(set.s_n) - 1)].at(it->second);
					uf[(stoi(set.s_n) - 1)].erase(it->second);
					(it->second)->returnSeat();
				}
			}
		}

		ofs << count << "\t0\tSuccess." << endl;
	}
}

void library::mem_add(string type, string name){
	if(!type.compare("Undergraduate")){
		map<string, undergraduate*>::iterator it = undergraduates.find(name);
		if(it == undergraduates.end()){
			undergraduates.insert(pair<string, undergraduate*>
					(name, new undergraduate(name)));	
		}
	}else if(!type.compare("Graduate")){
		map<string, graduate*>::iterator it = graduates.find(name);
		if(it == graduates.end()){
			graduates.insert(pair<string, graduate*>
					(name, new graduate(name)));	
		}
	}else if(!type.compare("Faculty")){
		map<string, faculty*>::iterator it = faculties.find(name);
		if(it == faculties.end()){
			faculties.insert(pair<string, faculty*>
					(name, new faculty(name)));	
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
/*
							  Check for resource
																			*/
//////////////////////////////////////////////////////////////////////////////

bool library::check_1(struct opset op, const int count, ofstream &ofs){
	bool ret = false;

	if(!(op.r_t).compare("Book")){
		map<string, book*>::iterator it = books.find(op.r_n);
		if(it == books.end()){
			ofs << count << "\t1\tNon exist resource." << endl;
			ret = true;
		}
	}else if(!(op.r_t).compare("Magazine")){
		pair<string, string> p = parseTitle(op.r_n);
		map<string, map<string, magazine*> >::iterator it = magazines.find(p.first);
		date c(p.second);
		date d(op.d);

		if(it == magazines.end()){
			ofs << count << "\t1\tNon exist resource." << endl;
			ret = true;
		}else if(!(op.o).compare("B") && (d - c < 0 || d - c > 360)){
			ofs << count << "\t1\tNon exist resource." << endl;
			ret = true;
		}
	}else if(!(op.r_t).compare("E-book")){
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
	}else if(!(op.m_t).compare("Graduate")){
		map<string, graduate*>::iterator it = graduates.find(op.m_n);
		if((it->second)->isOver()){
			ofs << count << "\t2\tExceeds your possible number of borrow. Possible # of borrows: "
				<< (it->second)->getLimit() << endl; 
			ret = true;
		}
	}else if(!(op.m_t).compare("Faculty")){
		map<string, faculty*>::iterator it = faculties.find(op.m_n);
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
	}else if(!(op.m_t).compare("Graduate")){
		map<string, graduate*>::iterator it = graduates.find(op.m_n);
		if(!((it->second)->isExist(op.r_t, op.r_n))){
			ofs << count << "\t3\tYou did not borrow this book." << endl;
			ret = true;
		}
	}else if(!(op.m_t).compare("Faculty")){
		map<string, faculty*>::iterator it = faculties.find(op.m_n);
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
	}else if(!(op.m_t).compare("Graduate")){
		map<string, graduate*>::iterator it = graduates.find(op.m_n);
		string tmp;
		if((it->second)->isExist(op.r_t, op.r_n, tmp)){
			ofs << count << "\t4\tYou already borrowed this book at " 
				<< tmp << endl;
			ret = true;
		}
	}else if(!(op.m_t).compare("Faculty")){
		map<string, faculty*>::iterator it = faculties.find(op.m_n);
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
		}else if((it->second)->isOccupied() != 0){
			date tmp((it->second)->getBorrowDate());
			tmp = tmp + 29;
			ofs << count << 
				"\t5\tOther member already borrowed this book. This book will be returned at " 
				<< tmp.getDate() << endl;
			ret = true;
		}
	}else if(!(op.r_t).compare("Magazine")){
		pair<string, string> p = parseTitle(op.r_n);
		map<string, map<string, magazine*> >::iterator rt = magazines.find(p.first);
		map<string, magazine*>::iterator it = (rt->second).find(op.r_n);
		if(it != (rt->second).end()){
			if((it->second)->isOccupied() == 1){
				date tmp((it->second)->getBorrowDate());
				tmp = tmp + 13;
				ofs << count << 
					"\t5\tOther member already borrowed this book. This book will be returned at " 
					<< tmp.getDate() << endl;
				ret = true;
			}else if((it->second)->isOccupied() != 0){
				date tmp((it->second)->getBorrowDate());
				tmp = tmp + 29;
				ofs << count << 
					"\t5\tOther member already borrowed this book. This book will be returned at " 
					<< tmp.getDate() << endl;
				ret = true;
			}
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
	}else if(!(op.m_t).compare("Graduate")){
		map<string, graduate*>::iterator it = graduates.find(op.m_n);
		string tmp;
		if((it->second)->isRestricted(op.d, tmp)){
			ofs << count << "\t6\tRestricted member until " 
				<< tmp << endl;
			ret = true;
		}
	}else if(!(op.m_t).compare("Faculty")){
		map<string, faculty*>::iterator it = faculties.find(op.m_n);
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
		}else if(!(op.m_t).compare("Graduate")){
			map<string, graduate*>::iterator it = graduates.find(op.m_n);

			if((it->second)->memOver((rt->second)->getSize())){
				ofs << count << "\t15\tExceeds your storage capacity." << endl;
				ret = true;
			}
		}else if(!(op.m_t).compare("Faculty")){
			map<string, faculty*>::iterator it = faculties.find(op.m_n);

			if((it->second)->memOver((rt->second)->getSize())){
				ofs << count << "\t15\tExceeds your storage capacity." << endl;
				ret = true;
			}
		}

	}
	return ret;
}

bool library::check_16(struct opset op, const int count, ofstream &ofs){
	bool ret = false;

	if(!(op.m_t).compare("Graduate")){
		map<string, graduate*>::iterator it = graduates.find(op.m_n);
		if((it->second)->isLate(op.d)){
			ofs << count << "\t16\tPreviously borrowed books are overdue, so borrow is limited." << endl;
			ret = true;
		}
	}else if(!(op.m_t).compare("Faculty")){
		map<string, faculty*>::iterator it = faculties.find(op.m_n);
		if((it->second)->isLate(op.d)){
			ofs << count << "\t16\tPreviously borrowed books are overdue, so borrow is limited." << endl;
			ret = true;
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
		}else if(!(op.r_t).compare("Magazine")){
			pair<string, string> p = parseTitle(op.r_n);
			map<string, map<string, magazine*> >::iterator it = magazines.find(p.first);
			map<string, magazine*>::iterator rt = (it->second).find(op.r_n);
			if(rt == (it->second).end()){
				(it->second).insert(rt, pair<string, magazine*>(op.r_n, new magazine(op.r_n)));	
				rt = (it->second).find(op.r_n);
			}
			(rt->second)->setUndergraduate(mt->second);
			(rt->second)->setBorrowDate(op.d);
		}else if(!(op.r_t).compare("E-book")){
			map<string, ebook*>::iterator rt = ebooks.find(op.r_n);
			(mt->second)->memSub((rt->second)->getSize());
		}
	}else if(!(op.m_t).compare("Graduate")){
		map<string, graduate*>::iterator mt = graduates.find(op.m_n);
		(mt->second)->addInfo(op.r_t, op.r_n, op.d);

		if(!(op.r_t).compare("Book")){
			map<string, book*>::iterator rt = books.find(op.r_n);
			(rt->second)->setGraduate(mt->second);
			(rt->second)->setBorrowDate(op.d);
		}else if(!(op.r_t).compare("Magazine")){
			pair<string, string> p = parseTitle(op.r_n);
			map<string, map<string, magazine*> >::iterator it = magazines.find(p.first);
			map<string, magazine*>::iterator rt = (it->second).find(op.r_n);
			if(rt == (it->second).end()){
				(it->second).insert(rt, pair<string, magazine*>(op.r_n, new magazine(op.r_n)));	
				rt = (it->second).find(op.r_n);
			}
			(rt->second)->setGraduate(mt->second);
			(rt->second)->setBorrowDate(op.d);
		}else if(!(op.r_t).compare("E-book")){
			map<string, ebook*>::iterator rt = ebooks.find(op.r_n);
			(mt->second)->memSub((rt->second)->getSize());
		}
	}else if(!(op.m_t).compare("Faculty")){
		map<string, faculty*>::iterator mt = faculties.find(op.m_n);
		(mt->second)->addInfo(op.r_t, op.r_n, op.d);

		if(!(op.r_t).compare("Book")){
			map<string, book*>::iterator rt = books.find(op.r_n);
			(rt->second)->setFaculty(mt->second);
			(rt->second)->setBorrowDate(op.d);
		}else if(!(op.r_t).compare("Magazine")){
			pair<string, string> p = parseTitle(op.r_n);
			map<string, map<string, magazine*> >::iterator it = magazines.find(p.first);
			map<string, magazine*>::iterator rt = (it->second).find(op.r_n);
			if(rt == (it->second).end()){
				(it->second).insert(rt, pair<string, magazine*>(op.r_n, new magazine(op.r_n)));	
				rt = (it->second).find(op.r_n);
			}
			(rt->second)->setFaculty(mt->second);
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
		}else if(!(op.r_t).compare("Magazine")){
			pair<string, string> p = parseTitle(op.r_n);
			map<string, map<string, magazine*> >::iterator it = magazines.find(p.first);
			map<string, magazine*>::iterator rt = (it->second).find(op.r_n);
			(rt->second)->freeUndergraduate();
		}else if(!(op.r_t).compare("E-book")){
			map<string, ebook*>::iterator rt = ebooks.find(op.r_n);
			(mt->second)->memAdd((rt->second)->getSize());
		}
	}else if(!(op.m_t).compare("Graduate")){
		map<string, graduate*>::iterator mt = graduates.find(op.m_n);
		(mt->second)->eraseInfo(op.r_t, op.r_n);

		if(!(op.r_t).compare("Book")){
			map<string, book*>::iterator rt = books.find(op.r_n);
			(rt->second)->freeGraduate();
		}else if(!(op.r_t).compare("Magazine")){
			pair<string, string> p = parseTitle(op.r_n);
			map<string, map<string, magazine*> >::iterator it = magazines.find(p.first);
			map<string, magazine*>::iterator rt = (it->second).find(op.r_n);
			(rt->second)->freeGraduate();
		}else if(!(op.r_t).compare("E-book")){
			map<string, ebook*>::iterator rt = ebooks.find(op.r_n);
			(mt->second)->memAdd((rt->second)->getSize());
		}
	}else if(!(op.m_t).compare("Faculty")){
		map<string, faculty*>::iterator mt = faculties.find(op.m_n);
		(mt->second)->eraseInfo(op.r_t, op.r_n);

		if(!(op.r_t).compare("Book")){
			map<string, book*>::iterator rt = books.find(op.r_n);
			(rt->second)->freeFaculty();
		}else if(!(op.r_t).compare("Magazine")){
			pair<string, string> p = parseTitle(op.r_n);
			map<string, map<string, magazine*> >::iterator it = magazines.find(p.first);
			map<string, magazine*>::iterator rt = (it->second).find(op.r_n);
			(rt->second)->freeFaculty();
		}else if(!(op.r_t).compare("E-book")){
			map<string, ebook*>::iterator rt = ebooks.find(op.r_n);
			(mt->second)->memAdd((rt->second)->getSize());
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
/*
							   Check for space
																			*/
//////////////////////////////////////////////////////////////////////////////

bool library::check_8(struct spset sp, const int count, ofstream &ofs){
	bool ret = false;
	if(!(sp.s_t).compare("StudyRoom")){
		if(stoi(sp.s_n) > 10 || stoi(sp.s_n) < 1)
			ret = true;
	}
	else{
		if(stoi(sp.s_n) > 3 || stoi(sp.s_n) < 1)
			ret = true;
	}
	
	if(ret){
		ofs << count << "\t8\tInvalid space id." << endl;
	}

	return ret;
}

bool library::check_9(struct spset sp, const int count, ofstream &ofs){
	bool ret = false;

	return ret;
}

bool library::check_10(struct spset sp, const int count, ofstream &ofs){
	bool ret = false;
	return ret;
}

bool library::check_11(struct spset sp, const int count, ofstream &ofs){
	bool ret = false;
	return ret;
}

bool library::check_12(struct spset sp, const int count, ofstream &ofs){
	bool ret = false;
	return ret;
}

bool library::check_13(struct spset sp, const int count, ofstream &ofs){
	bool ret = false;
	return ret;
}

bool library::check_14(struct spset sp, const int count, ofstream &ofs){
	bool ret = false;
	return ret;
}

void library::spcReset(string d){
	h_date base(d);
	for(auto it : rooms){
		if(it->getStatus() == FREE){
			continue;
		}

		h_date comp(it->getTime());

		if(base - comp > 0 || base.getHour() > 18 ||  base.getHour() - comp.getHour() > it->getDuring()){
			if(!(it->getMemType()).compare("Undergraduate")){
				map<string, undergraduate*>::iterator ut = undergraduates.find(it->getMemName());
				(ut->second)->returnStudyRoom();
			}
			it->returnRoom();
		}
	}

	int tl[3] = {24, 21, 18};

	for(int i = 0; i < 3; i++){
		for(auto it = uf[i].begin(); it != uf[i].end();){
			h_date comp((it->second)->getTime());
			if(base - comp > 0 || base.getHour() > tl[i] ||  base.getHour() - comp.getHour() > (it->second)->getDuring()){
				(it->first)->returnSeat();
				delete it->second;
				uf[i].erase(it++);
			}
			else if((it->second)->getStatus() == EMPTY && base.getHour() - (it->second)->getEtime() > 1){
				(it->first)->returnSeat();
				delete it->second;
				uf[i].erase(it++);

			}
			else{
				++it;
			}
		}
	}
}
