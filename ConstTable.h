//
// Created by QZQ on 2018/6/9.
//

#ifndef SPLC_CONSTTABLE_H
#define SPLC_CONSTTABLE_H

#include <string>
#include <stack>
#include <map>

using namespace std;

union ConstValueUnion {
	double real;
	int integer;
	char ch;
};

class ConstTable {
	map <string, stack<ConstValueUnion>> table;

	int getInt(const string &name) {
		return table.at(name).top().integer;
	}

	double getReal(const string &name) {

	}

	char getChar(const string &name) {
		
	}
};

#endif //SPLC_CONSTTABLE_H
