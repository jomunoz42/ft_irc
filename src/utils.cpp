
#include "irc.hpp"

std::vector<std::string> split(std::string str, std::string del) 
{
	std::vector<std::string> strs;
	size_t pos;
	while ((pos = str.find(del)) != std::string::npos) 
	{
		if (pos)
			strs.push_back(str.substr(0, pos));
		str.erase(0, pos + del.length());
	}
	if (!str.empty())
		strs.push_back(str);
	return (strs);
}