#pragma once
#include <string>
#include <vector>
#include <sstream>

class StringUtils
{
public:
	static std::vector<std::string> split(const std::string& s, char delimiter);
};

