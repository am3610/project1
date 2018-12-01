#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
#include "library.h"

#define SR_SIZE			10
#define SSIZE			50

#define FREE			0
#define EMPTY			1
#define OCCUPIED		2

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
			magazines.insert(n);
		}else if(!t.compare("E-book")){
			pair<string, int> p = parseTitle(n);
			ebooks.insert(pair<string, ebook*>(p.first, new ebook(p.first, p.second)));
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
					uf[(stoi(set.s_n) - 1)].at(it->second)->empty(set.d);
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

	h_date base(sp.d);
	int bh = base.getHour();

	if(!(sp.s_t).compare("StudyRoom")){
		if(bh < 9 || bh >= 18){
			ret = true;
			ofs << count << "\t9\tThis space is not available now. Available from 09 to 18." << endl;
		}
	}
	else{
		if(stoi(sp.s_n) == 2 && (bh < 9 || bh >= 21)){
			ret = true;
			ofs << count << "\t9\tThis space is not available now. Available from 09 to 21." << endl;
		}
		else if(stoi(sp.s_n) == 3 && (bh < 9 || bh >= 18)){
			ret = true;
			ofs << count << "\t9\tThis space is not available now. Available from 09 to 18." << endl;
		}
	}

	return ret;
}

bool library::check_10(struct spset sp, const int count, ofstream &ofs){
	bool ret = false;

	if(!(sp.m_t).compare("Undergraduate")){
		map<string, undergraduate*>::iterator it = undergraduates.find(sp.m_n);
		
		if(!(sp.s_t).compare("StudyRoom") && (it->second)->retStudyRoom() != stoi(sp.s_n)){
			ret = true;
		}
		
		if(!(sp.s_t).compare("Seat") && (it->second)->retSeat() != stoi(sp.s_n)){
			ret = true;
		}
	}

	if(ret){
		ofs << count << "\t10\tYou did not borrow this space." << endl;
	}

	return ret;
}

bool library::check_11(struct spset sp, const int count, ofstream &ofs){
	bool ret = false;

	if(!(sp.m_t).compare("Undergraduate")){
		map<string, undergraduate*>::iterator it = undergraduates.find(sp.m_n);
		
		if(!(sp.s_t).compare("StudyRoom") && (it->second)->retStudyRoom() != 0){
			ret = true;
		}
		
		if(!(sp.s_t).compare("Seat") && (it->second)->retSeat() != 0){
			ret = true;
		}
	}

	if(ret){
		ofs << count << "\t11\tYou already borrowed this kind of space." << endl;
	}

	return ret;
}

bool library::check_12(struct spset sp, const int count, ofstream &ofs){
	bool ret = false;

	if(!(sp.s_t).compare("StudyRoom")){
		if(stoi(sp.n_m) > 6){
			ret = true;
		}
	}
	else{	
		if(stoi(sp.n_m) > 1){
			ret = true;
		}
	}

	if(ret){
		ofs << count << "\t12\tExceed available number." << endl;
	}

	return ret;
}

bool library::check_13(struct spset sp, const int count, ofstream &ofs){
	bool ret = false;

	if(!(sp.s_t).compare("StudyRoom")){
		if(stoi(sp.t) > 3){
			ret = true;
		}
	}

	else{	
		if(!(sp.m_t).compare("Undergraduate")){
			if(stoi(sp.t) > 3){
				ret = true;
			}
		}
	}

	if(ret){
		ofs << count << "\t13\tExceed available time." << endl;
	}

	return ret;
}

bool library::check_14(struct spset sp, const int count, ofstream &ofs){
	bool ret = false;

	if(!(sp.s_t).compare("StudyRoom")){
		if((rooms.at(stoi(sp.s_n) - 1)->getStatus() != FREE)){
			h_date hour(rooms.at(stoi(sp.s_n) - 1)->getTime());

			int h = hour.getHour();
			h = h + rooms.at(stoi(sp.s_n) - 1)->getDuring();

			string ph;
			if(h > 18){
				ph = to_string(18);
			}
			else if(h < 10){
				ph = "0" + to_string(h);
			}
			else{
				ph = to_string(h);
			}

			ofs << count << "\t14\tThere is no remain space. This space is available after " <<
				ph << "." << endl;
			ret = true;
		}
	}
	else{
		int tl[3] = {24, 21, 18};

		int idx = stoi(sp.s_n) - 1; 

		int h = tl[idx];

		if(uf[idx].size() == 50){
			for(auto it = uf[idx].begin(); it != uf[idx].end(); it++){
				h_date hour((it->second)->getTime());
				int t = hour.getHour() + (it->second)->getDuring();
				h = min(h, t);
			}

			string ph;

			if(h >= 24){
				ph = "00";
			}
			else if(h < 10){
				ph = "0" + to_string(h);
			}
			else{
				ph = to_string(h);
			}

			ofs << count << "\t14\tThere is no remain space. This space is available after " <<
				ph << "." << endl;
			ret = true;
		}
	}
	return ret;
}

void library::spcReset(string d){
	h_date base(d);
	for(auto it : rooms){
		if(it->getStatus() == FREE){
			continue;
		}

		h_date comp(it->getTime());

		if(base - comp > 0 || base.getHour() >= 18 ||  base.getHour() - comp.getHour() >= it->getDuring()){
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
			if(base - comp > 0 || base.getHour() >= tl[i] ||  base.getHour() - comp.getHour() >= (it->second)->getDuring()){
				(it->first)->returnSeat();
				delete it->second;
				uf[i].erase(it++);
			}
			else if((it->second)->getStatus() == EMPTY && base.getHour() - (it->second)->getEtime() >= 1){
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
