
#include "irc.hpp"

Channel::Channel(std::string name) : _channel_name(name) {
	// this->_channel_commands.insert(std::make_pair("KICK", commandKick));
	// this->_channel_commands.insert(std::make_pair("INVITE", commandInvite));
	// this->_channel_commands.insert(std::make_pair("TOPIC", commandTopic));
	// this->_channel_commands.insert(std::make_pair("MODE", commandMode));
}

Channel::~Channel() {}

Channel::Channel(const Channel& other) : _channel_name(other._channel_name), _channel_topic(other._channel_topic), \
_channel_password(other._channel_password), _users(other._users), _operators(other._operators), _channel_commands(other._channel_commands) {}

Channel& Channel::operator=(const Channel& other) {
	if (this != &other) {
		this->_channel_name = other._channel_name;
		this->_channel_topic = other._channel_topic;
		this->_channel_password = other._channel_password;
		this->_users = other._users;
		this->_operators = other._operators;
		this->_channel_commands = other._channel_commands;
	}
	return (*this);
}

void Channel::setTopic(const std::string new_topic) {
	if (new_topic.empty())
		return ;
	this->_channel_topic = new_topic;
}

std::string Channel::getTopic(void) {
	return (this->_channel_topic);
}

std::vector<Client*> Channel::getUsers(void) {
	return (this->_users);
}

void Channel::removeUser(Client &client) {
	for (std::vector<Client*>::iterator i = this->_users.begin(); i != this->_users.end(); ++i) {
		if (*i == &client) {
			this->_users.erase(i);
			std::cout << "User " << client.getNickname() << " has been removed from " << this->_channel_name << std::endl;
			break ;
		}
	}
	for (std::vector<Client*>::iterator j = this->_operators.begin(); j != this->_operators.end(); ++j) {
		if (*j == &client) {
			this->_operators.erase(j);
			std::cout << "Operator " << client.getNickname() << " has been removed from " << this->_channel_name << std::endl;
			break ;
		}
	}
}


// void Channel::commandKick(std::string user) {

// }

// void Channel::commandInvite(std::string user) {

// }

// void Channel::commandTopic(std::string new_topic) {

// }

// void Channel::commandMode(std::string new_mode) {

// }