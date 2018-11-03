#ifndef _MEMBER_H
#define _MEMBER_H
#include <string>
#include <vector>
#include "date.h"

struct resInfo{
	std::string type;
	std::string title;

	resInfo(std::string type, std::string title){
		this->type = type;
		this->title = title;
	}
};

class member{
	private:
		std::string name;

	protected:
		date paneltyDate;
		
	public:
		member(std::string name);
		void setPaneltyDate(date paneltyDate);
};

class undergraduate : public member{
	private:
		std::vector<struct resInfo*> info;
		int limit;

	public:
		undergraduate(std::string name);
		void addInfo(std::string type, std::string title);
		void eraseInfo(std::string type, std::string title);
		bool isExist(std::string type, std::string title);
		bool isOver();
		int getLimit();
		bool isRestricted(const std::string &borrDate, std::string &tmp);
};

#endif
