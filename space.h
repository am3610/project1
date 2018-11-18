#ifndef _SPACE_H
#define _SPACE_H

#include <queue>
#include <string>
#include <utility>
#include "date.h"

class space{
	protected:
		int status;
		std::string time;
		std::string m_type;
		std::string m_name;
		int during;

	public:
		int getStatus();
		int getDuring();
		std::string getTime();
		std::string getMemType();
		std::string getMemName();
};

class studyRoom : public space{
	public:
		studyRoom();
		void borrowRoom(std::string, std::string, std::string, std::string);
		void returnRoom();
	
};

class seat : public space{
	private:
		std::string etime;

	public:
		seat(std::string, std::string, std::string, std::string);
		void empty(std::string);
		void comeback();
		int getEtime();
};

#endif
