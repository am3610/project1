#include <vector>
#include "date.h"

using namespace std;

date::date(){

}

date::date(int year, int month, int day){
	this->year = year;
	this->month = month;
	this->day = day;
}

date::date(string d){
	string tmp;
	vector<int> buf;
	for(auto s : d){
		if(s == '/'){
			buf.push_back(stoi(tmp));
			tmp.clear();
		}
		else{
			tmp.push_back(s);
		}
	}
	buf.push_back(stoi(tmp));

	year = buf.at(0);
	month = buf.at(1);
	day = buf.at(2);
}

void date::operator=(const date &d){
	year = d.year;
	month = d.month;
	day = d.day;
}

date date::operator+(const int &n){
	date tmp;

	tmp.day = day + n;
	tmp.month = month + (tmp.day - 1)/30;
	tmp.year = year + (tmp.month - 1)/12;

	tmp.day = ((tmp.day - 1) % 30) + 1;
	tmp.month = ((tmp.month - 1) % 12) + 1;

	return date(tmp.year, tmp.month, tmp.day);
}

int date::operator-(const date &d){
	int y_dif = (year - d.year)*360;
	int m_dif = (month - d.month)*30;
	int d_dif = day - d.day;
	return y_dif + m_dif + d_dif;
}

int date::getYear(){ return year; }
int date::getMonth(){ return month; }
int date::getDay(){ return day; }
