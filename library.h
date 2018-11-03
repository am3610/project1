#ifndef _LIBRARY_H
#define _LIBRARY_H
#include <string>
#include <map>
#include "member.h"
#include "resource.h"
#include "date.h"

struct opset{
	std::string d;
	std::string r_t;
	std::string r_n;
	std::string o;
	std::string m_t;
	std::string m_n;
};

class library{
	private:
		std::map<std::string, undergraduate*> undergraduates;
		std::map<std::string, book*> books;
		
		void mem_add(std::string type, std::string name);
		bool check_1(struct opset op, const int count);
		bool check_2(struct opset op, const int count);
		bool check_3(struct opset op, const int count);
		bool check_4(struct opset op, const int count);
		bool check_5(struct opset op, const int count);
		bool check_6(struct opset op, const int count);
		bool check_7(struct opset op, const int count);

		void borrowRes(struct opset op);
		void returnRes(struct opset op);

	public:
		library(char* res);
		void execute(char *inp);
};

#endif
