#include "space.h"
#include <iostream>

#define FREE			0
#define	EMPTY			1
#define OCCUPIED		2

using namespace std;

studyRoom::studyRoom(){
	status = FREE;
}

int space::getStatus(){
	return status;
}

int space::getDuring(){
	return during;
}

string space::getTime(){
	return time;
}

string space::getMemType(){
	return m_type;
}

string space::getMemName(){
	return m_name;
}

void studyRoom::borrowRoom(string t, string m_t, string m_n, string d){
	status = OCCUPIED;
	time = t;
	m_type = m_t;
	m_name = m_n;
	during = stoi(d);
}

void studyRoom::returnRoom(){
	status = FREE;
}


seat::seat(string t, string m_t, string m_n, string d){
	status = OCCUPIED;
	time = t;
	m_type = m_t;
	m_name = m_n;
	during = stoi(d);
}

void seat::empty(string et){
	status = EMPTY;
	etime = et;
}

void seat::comeback(){
	status = OCCUPIED;
}

int seat::getEtime(){
	h_date ret(etime);
	return ret.getHour();
}
