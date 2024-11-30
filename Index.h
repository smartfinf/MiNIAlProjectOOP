#pragma once
#include<string>
class Index
{
	std::string name;
	void set_name(std::string name) {
		this->name = name;
	}
	std::string get_name() {
		return this->name;
	}
};