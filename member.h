#ifndef _MEMBER_H
#define _MEMBER_H
#include <string>
#include <vector>
#include "date.h"

struct resInfo{
	std::string type;
	std::string title;
	std::string date;

	resInfo(std::string type, std::string title, std::string date){
		this->type = type;
		this->title = title;
		this->date = date;
	}
};

class member{
	protected:
		std::string name;						// Member's name (priority key)
		date paneltyDate;						// Restricted date
		std::vector<struct resInfo*> info;		// Information about resources borrowed by the user.
		int limit;								// The amount of resources a user can borrow.
		int memory;

	public:
		member(std::string);
		void setPaneltyDate(date);

		void addInfo(std::string, std::string, std::string);
		void eraseInfo(std::string, std::string);
		bool isExist(std::string, std::string);
		bool isExist(std::string, std::string, std::string&);
		bool isOver();
		int getLimit();
		bool isRestricted(const std::string&, std::string&);
		void memSub(int);
		void memAdd(int);
		bool memOver(int);
};

class undergraduate : public member{

	public:
		undergraduate(std::string name);
};

class graduate : public member{

	public:
		graduate(std::string name);
};

class faculty : public member{

	public:
		faculty(std::string name);
};
#endif
