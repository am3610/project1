#ifndef _RESOURCE_H
#define _RESOURCE_H
#include <string>
#include "member.h"
#include "date.h"

class resource{
	protected:
		std::string title;
		date borrowDate;
		undergraduate *u_borrower;

	public:
		resource(std::string);
		void setBorrowDate(std::string);
		std::string getBorrowDate();
		std::string getTitle();

		void setUndergraduate(undergraduate*);
		void freeUndergraduate();
		bool isOccupied();
		bool isLate(const std::string&, const std::string&, std::string&);
};

class book : public resource{
	public:
		book(std::string);
};

class magazine : public resource{
	public:
		magazine(std::string);

};

class ebook : public resource{
	private:
		int size;

	public:
		ebook(std::string, int);
		int getSize();
};

#endif
