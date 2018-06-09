//
// Created by QZQ on 2018/6/9.
//

#ifndef SPLC_CONSTTABLE_H
#define SPLC_CONSTTABLE_H

#include <string>
#include <stack>
#include <map>

union ConstValueUnion {
	double real;
	int integer;
	char ch;
};

class ConstTable {
	std::map <std::string, std::stack<ConstValueUnion>> table;

	int getInt(const std::string &name) {
		return table.at(name).top().integer;
	}

	double getReal(const std::string &name) {

	}

	char getChar(const std::string &name) {

	}
};

#endif //SPLC_CONSTTABLE_H
