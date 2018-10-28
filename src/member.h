#ifndef _MEMBER_H
#define _MEMBER_H
#include <string>
#include <vector>
#include "resource.h"
#include "date.h"

class member{
	private:
		int id;
		int limit;
		std::string name;
		
	public:
		member(int id, std::string name);
		std::string getName();
		void setLimit(int limit);
};

class undergraduate : public member{
	private:
		std::vector<book> borrow;

	public:
		undergraduate(int id, std::string name);
};

#endif
