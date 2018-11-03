#ifndef _RESOURCE_H
#define _RESOURCE_H
#include <string>
#include "member.h"
#include "date.h"

class resource{
	private:
		std::string title;
		date borrowDate;

	public:
		resource(std::string title);
		void setBorrowDate(std::string borrowDate);
		std::string getBorrowDate();
		std::string getTitle();
};

class book : public resource{
	private:
		undergraduate *u_borrower;

	public:
		book(std::string title);
		void setUndergraduate(undergraduate *ug);
		void freeUndergraduate();
		int isOccupied();
		bool isLate(const std::string &retDate, const std::string &mType, 
				std::string &tmp);
};

#endif
