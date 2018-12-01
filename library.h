#ifndef _LIBRARY_H
#define _LIBRARY_H
#include <string>
#include <set>
#include <map>
#include <fstream>
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
		std::map<std::string, ebook*> ebooks;
		std::set<std::string> magazines;
		
		void mem_add(std::string, std::string);
		bool check_1(struct opset, const int, std::ofstream&);
		bool check_2(struct opset, const int, std::ofstream&);
		bool check_3(struct opset, const int, std::ofstream&);
		bool check_4(struct opset, const int, std::ofstream&);
		bool check_5(struct opset, const int, std::ofstream&);
		bool check_6(struct opset, const int, std::ofstream&);
		bool check_7(struct opset, const int, std::ofstream&);

		void borrowRes(struct opset);
		void returnRes(struct opset);

	public:
		library(char*);
		void execute(char*);
};

#endif
