#ifndef _BOOK_H
#define _BOOK_H
#include <string>

struct date{
	int year;
	int month;
	int day;
};

class resource{
	private:
		struct date *borrow;
		std::string title;

	public:
		resource(std::string title);
		void dateParse(std::string date);
};

class book : public resource{
	private:

	public:
		book(std::string title);
};

#endif
