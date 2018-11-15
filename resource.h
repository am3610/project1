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
		resource(std::string title);
		void setBorrowDate(std::string borrowDate);
		std::string getBorrowDate();
		std::string getTitle();

		void setUndergraduate(undergraduate *ug);
		void freeUndergraduate();
		int isOccupied();
		bool isLate(const std::string &retDate, const std::string &mType, 
				std::string &tmp);
};

class book : public resource{
	public:
		book(std::string title);
};

#endif
