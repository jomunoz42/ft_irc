
#include "irc.hpp"

Channel::Channel(std::string name)
		: _channel_name(name),
		  _invite_only(false),
		  _topic_restricted(false),
		  _has_password(false),
		  _has_limit(false),
		  _user_limit(0)
{}

Channel::Channel(const Channel& other)
		: _channel_name(other._channel_name),
		  _channel_topic(other._channel_topic),
		  _channel_password(other._channel_password),
		  _users(other._users),
		  _operators(other._operators),
		  _invited(other._invited),
		  _invite_only(other._invite_only),
		  _topic_restricted(other._topic_restricted),
		  _has_password(other._has_password),
		  _has_limit(other._has_limit),
		  _user_limit(other._user_limit)
{}

Channel& Channel::operator=(const Channel& other) 
{
	if (this != &other) 
	{
		this->_channel_name = other._channel_name;
		this->_channel_topic = other._channel_topic;
		this->_channel_password = other._channel_password;
		this->_users = other._users;
		this->_operators = other._operators;
		this->_invited = other._invited;
		this->_invite_only = other._invite_only;
		this->_topic_restricted = other._topic_restricted;
		this->_has_password = other._has_password;
		this->_has_limit = other._has_limit;
		this->_user_limit = other._user_limit;
	}
	return (*this);
}

Channel::~Channel() {}

std::string 		 Channel::getTopic(void) const  {return (this->_channel_topic);}

std::vector<Client*> Channel::getUsers(void) {return (this->_users);}

std::string 		 Channel::getNameList(void) const 
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

bool Channel::isInviteOnly(void) const{return (this->_invite_only);}

bool Channel::isTopicRestricted(void) const {return (this->_topic_restricted);}

bool Channel::hasPassword(void) const {return (this->_has_password);}

bool Channel::hasLimit(void) const {return (this->_has_limit);}

size_t Channel::getUserLimit(void) const {return (this->_user_limit);}

void Channel::setInviteOnly(bool status) {this->_invite_only = status;}

void Channel::setTopicRestricted(bool status) {this->_topic_restricted = status;}

void Channel::setHasPassword(bool status) {this->_has_password = status;}

void Channel::setHasLimit(bool status) {this->_has_limit = status;}

void Channel::setUserLimit(size_t limit) {this->_user_limit = limit;}

std::string Channel::getPassword(void) const {return (this->_channel_password);}

void Channel::setPassword(const std::string &password) {this->_channel_password = password;}