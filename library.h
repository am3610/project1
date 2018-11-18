#ifndef _LIBRARY_H
#define _LIBRARY_H
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <vector>
#include "member.h"
#include "resource.h"
#include "space.h"
#include "date.h"

struct opset{
	std::string d;
	std::string r_t;
	std::string r_n;
	std::string o;
	std::string m_t;
	std::string m_n;
};

struct spset{
	std::string d;
	std::string s_t;
	std::string s_n;
	std::string o;
	std::string m_t;
	std::string m_n;
	std::string n_m;
	std::string t;
};

class library{
	private:
		int count;

		std::map<std::string, undergraduate*> undergraduates;
		std::map<std::string, graduate*> graduates;
		std::map<std::string, faculty*> faculties;
		std::map<std::string, book*> books;
		std::map<std::string, ebook*> ebooks;
		std::map<std::string, std::map<std::string, magazine*> > magazines;

		std::map<std::string, seat*> f1; 
		std::map<std::string, seat*> f2; 
		std::map<std::string, seat*> f3;
		std::map<undergraduate*, seat*> uf[3]; 
		std::vector<studyRoom*> rooms;
		
		void resManager(std::string, std::ofstream&);
		void spcManager(std::string, std::ofstream&);

		void mem_add(std::string, std::string);

		bool check_1(struct opset, const int, std::ofstream&);
		bool check_2(struct opset, const int, std::ofstream&);
		bool check_3(struct opset, const int, std::ofstream&);
		bool check_4(struct opset, const int, std::ofstream&);
		bool check_5(struct opset, const int, std::ofstream&);
		bool check_6(struct opset, const int, std::ofstream&);
		bool check_7(struct opset, const int, std::ofstream&);
		bool check_15(struct opset, const int, std::ofstream&);
		bool check_16(struct opset, const int, std::ofstream&);

		void borrowRes(struct opset);
		void returnRes(struct opset);

		bool check_8(struct spset, const int, std::ofstream&);
		bool check_9(struct spset, const int, std::ofstream&);
		bool check_10(struct spset, const int, std::ofstream&);
		bool check_11(struct spset, const int, std::ofstream&);
		bool check_12(struct spset, const int, std::ofstream&);
		bool check_13(struct spset, const int, std::ofstream&);
		bool check_14(struct spset, const int, std::ofstream&);

		void spcReset(std::string); 

	public:
		library(char**);
};

#endif
