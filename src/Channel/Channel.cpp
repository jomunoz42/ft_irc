
#include "irc.hpp"

Channel::Channel(std::string name)
	: _channel_name(name),
	  _invite_only(false),
	  _topic_restricted(false),
	  _has_password(false),
	  _has_limit(false),
	  _user_limit(0)
{
	// this->_channel_commands.insert(std::make_pair("KICK", commandKick));
	// this->_channel_commands.insert(std::make_pair("INVITE", commandInvite));
	// this->_channel_commands.insert(std::make_pair("TOPIC", commandTopic));
	// this->_channel_commands.insert(std::make_pair("MODE", commandMode));
}

Channel::Channel(const Channel& other) : _channel_name(other._channel_name), _channel_topic(other._channel_topic), \
_channel_password(other._channel_password), _users(other._users), _operators(other._operators), _channel_commands(other._channel_commands) {}

Channel& Channel::operator=(const Channel& other) 
{
	if (this != &other) 
	{
		this->_channel_name = other._channel_name;
		this->_channel_topic = other._channel_topic;
		this->_channel_password = other._channel_password;
		this->_users = other._users;
		this->_operators = other._operators;
		this->_channel_commands = other._channel_commands;
	}
	return (*this);
}

Channel::~Channel() {}

std::string Channel::getTopic(void) const  {return (this->_channel_topic);}

std::vector<Client*> Channel::getUsers(void) {return (this->_users);}

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

void Channel::setTopic(const std::string new_topic) 
{
	if (new_topic.empty())
		return ;
	this->_channel_topic = new_topic;
}