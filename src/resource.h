#ifndef _BOOK_H
#define _BOOK_H
#include <string>
#include "date.h"

class resource{
	private:
		date *borrow;
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
