#pragma once

#include<iostream>
#include<fstream>

class config
{
public:
	std::string filterName;
	int up, left, bottom, right;

	config() : up(0), left(0), bottom(1), right(1){}
	void readCfg(const std::string&);
};