#ifndef _DATE_H
#define _DATE_H
#include <string>

class date{
	private:
		int year;
		int month;
		int day;
		int hour;

	public:
		date();
		date(int year, int month, int day);
		date(std::string d);
		void operator=(const date& d);
		date operator+(const int& n);
		int operator-(const date &d);
		std::string getDate();
};

#endif
