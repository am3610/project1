#ifndef _MEMBER_H
#define _MEMBER_H
#include <string>
#include <vector>
#include "date.h"

struct resInfo{
	std::string type;
	std::string title;
	std::string date;
	int size;

	resInfo(std::string type, std::string title, std::string date){
		this->type = type;
		this->title = title;
		this->date = date;
	}

	resInfo(std::string type, std::string title, std::string date, int size){
		this->type = type;
		this->title = title;
		this->date = date;
		this->size = size;
	}
};

class member{
	protected:
		std::string name;						// Member's name (priority key)
		date paneltyDate;						// Restricted date
		std::vector<struct resInfo*> info;		// Information about resources borrowed by the user.
		int limit;								// The amount of resources a user can borrow.
		int memory;
		int studyRoom_no;
		int seatFloor;
		int during;
		
	public:
		member(std::string);
		void setPaneltyDate(date);

		void addInfo(std::string, std::string, std::string);
		void addInfo(std::string, std::string, std::string, int);
		void eraseInfo(std::string, std::string);
		bool isExist(std::string, std::string);
		bool isExist(std::string, std::string, std::string&);
		bool isOver();
		int getLimit();
		bool isRestricted(const std::string&, std::string&);
		void memSub(int);
		void memAdd(int);
		bool memOver(int);
		void expireEbook(std::string);
		bool isLate(std::string);

		void borrowStudyRoom(int);
		void returnStudyRoom();
		void borrowSeat(int);
		void returnSeat();

		int retStudyRoom();
		int retSeat();
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
