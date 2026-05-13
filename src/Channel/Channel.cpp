
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

Channel& Channel::operator=(const Channel& other) 
{
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