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
	protected:
		std::string name;						// Member's name (priority key)
		date paneltyDate;						// Restricted date
		std::vector<struct resInfo*> info;		// Information about resources borrowed by the user.
		int limit;								// The amount of resources a user can borrow.
		int studyRoom_no;
		int seatFloor;
		int memory;

	public:
		member(std::string);
		void setPaneltyDate(date);

		void addInfo(std::string, std::string);
		void eraseInfo(std::string, std::string);
		bool isExist(std::string, std::string);
		bool isOver();
		int getLimit();
		bool isRestricted(const std::string&, std::string&);

		void borrowStudyRoom(int);
		void returnStudyRoom();
		void borrowSeat(int);
		void returnSeat();

		int retStudyRoom();
		int retSeat();

		void memSub(int);
		void memAdd(int);
		bool memOver(int);
};

class undergraduate : public member{

	public:
		undergraduate(std::string name);
};

#endif
