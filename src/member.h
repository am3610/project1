#ifndef _MEMBER_H
#define _MEMBER_H
#include <string>

class member{
	private:
		std::string name;
		
	public:
		member(std::string name);
		std::string getName();
};

class undergraduate : public member{
	private:	
		
	public:
		undergraduate(std::string name);
};

#endif
