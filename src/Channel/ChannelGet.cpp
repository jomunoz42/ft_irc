
#include "irc.hpp"

std::string Channel::getTopic(void) const 
{
	return (this->_channel_topic);
}

std::vector<Client*> Channel::getUsers(void) 
{
	return (this->_users);
}

std::string Channel::getNameList(void) const 
{
	std::stringstream build_list;
	size_t n_users = this->_users.size();
	for (size_t i = 0; i < n_users; ++i) 
	{
		build_list << this->_users.at(i)->getNickname();
		if (i + 1 < n_users)
			build_list << " ";
	}
	std::string name_list = build_list.str();
	return (name_list);
}