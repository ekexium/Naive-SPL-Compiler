//
// Created by QZQ on 2018/6/9.
//

#ifndef SPLC_CONSTTABLE_H
#define SPLC_CONSTTABLE_H

#include <string>
#include <list>
#include <map>
#include <iostream>

struct ConstValueUnion {
	double real{};
	int integer{};
	char ch{};

	explicit ConstValueUnion(int i) : integer(i) {}

	explicit ConstValueUnion(double r) : real(r) {}

	explicit ConstValueUnion(char c) : ch(c) {}

};

enum ConstType {
	integer, real, ch
};

class ConstTable {
public:
	void printTable() {
		std::cout << "const table : \n============================";
		for (auto bucket : table) {
			std::cout << bucket.first << ": ";
			for (auto item : bucket.second)
				switch (item.first) {
					case ConstType::integer:
						std::cout << item.second.integer << ", ";
						break;
					case ConstType::real:
						std::cout << item.second.real << ", ";
						break;
					case ConstType::ch:
						std::cout << item.second.ch << ", ";
						break;
				}
			std::cout << std::endl;
		}
	}

	std::map<std::string, std::list<std::pair<ConstType, ConstValueUnion>>> table;

	int getInt(const std::string &name) {
		auto item = table.at(name).back();
		assert(item.first == ConstType::integer);
		return table.at(name).back().second.integer;
	}

	double getReal(const std::string &name) {
		auto item = table.at(name).back();
		assert(item.first == ConstType::real);
		return table.at(name).back().second.real;
	}

	char getChar(const std::string &name) {
		auto item = table.at(name).back();
		assert(item.first == ConstType::ch);
		return table.at(name).back().second.ch;
	}

	void setInt(const std::string &name, int i) {
		if (table.find(name) != table.end()) {
			table.at(name).push_back(std::pair(ConstType::integer, ConstValueUnion(i)));
		} else {
			table.insert(std::make_pair(name, std::list<std::pair<ConstType, ConstValueUnion>>()));
			table.at(name).push_back(std::make_pair(ConstType::integer, ConstValueUnion(i)));
		}
	}

	void setReal(const std::string &name, double r) {
		if (table.find(name) != table.end()) {
			table.at(name).push_back(std::pair(ConstType::real, ConstValueUnion(r)));
		} else {
			table.insert(std::make_pair(name, std::list<std::pair<ConstType, ConstValueUnion>>()));
			table.at(name).push_back(std::make_pair(ConstType::real, ConstValueUnion(r)));
		}
	}

	void setChar(const std::string &name, char c) {
		if (table.find(name) != table.end()) {
			table.at(name).push_back(std::pair(ConstType::ch, ConstValueUnion(c)));
		} else {
			table.insert(std::make_pair(name, std::list<std::pair<ConstType, ConstValueUnion>>()));
			table.at(name).push_back(std::make_pair(ConstType::ch, ConstValueUnion(c)));
		}
	}
};

#endif //SPLC_CONSTTABLE_H
