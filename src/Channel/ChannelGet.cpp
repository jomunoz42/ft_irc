
#include "irc.hpp"

std::string Channel::getTopic(void) {
	return (this->_channel_topic);
}

std::vector<Client*> Channel::getUsers(void) {
	return (this->_users);
}