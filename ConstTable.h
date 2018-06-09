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
public:
	std::map <std::string, std::pair<int, std::stack<ConstValueUnion>>> table;

	int getInt(const std::string &name) {
		return table.at(name).second.top().integer;
	}

	double getReal(const std::string &name) {
		return table.at(name).second.top().real;
	}

	char getChar(const std::string &name) {
		return table.at(name).second.top().ch;
	}


};

#endif //SPLC_CONSTTABLE_H
