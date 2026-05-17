
#include "irc.hpp"

void Channel::addUser(Client &client) 
{
	if (this->hasUser(client))
		return ;

	this->_users.push_back(&client);
}

void Channel::removeUser(Client &client) 
{
	for (std::vector<Client*>::iterator i = this->_users.begin(); i != this->_users.end(); ++i) 
	{
		if (*i == &client) 
		{
			this->_users.erase(i);
			break ;
		}
	}
}

bool Channel::hasUser(Client &client)
{
	for (size_t i = 0; i < this->_users.size(); ++i)
	{
		if (this->_users[i] == &client)
			return (true);
	}
	return (false);
}

void Channel::addOperator(Client &client) 
{
	if (!this->hasUser(client))
		return ;
	if (this->hasOperator(client))
		return ;

	this->_operators.push_back(&client);
}

void Channel::removeOperator(Client &client) 
{
	for (std::vector<Client*>::iterator j = this->_operators.begin(); j != this->_operators.end(); ++j) 
	{
		if (*j == &client) 
		{
			this->_operators.erase(j);
			break ;
		}
	}
}

bool Channel::hasOperator(Client &client)
{
	for (size_t i = 0; i < this->_operators.size(); ++i)
	{
		if (this->_operators[i] == &client)
			return (true);
	}
	return (false);
}

void 	Channel::addInvited(Client &client)
{
	if (this->hasInvited(client))
		return ;

	this->_invited.push_back(&client);
}

void 	Channel::removeInvited(Client &client)
{
	for (std::vector<Client*>::iterator j = this->_invited.begin(); j != this->_invited.end(); ++j) 
	{
		if (*j == &client) 
		{
			this->_invited.erase(j);
			break ;
		}
	}
}

bool 	Channel::hasInvited(Client &client)
{
	for (size_t i = 0; i < this->_invited.size(); ++i)
	{
		if (this->_invited[i] == &client)
			return (true);
	}
	return (false);
}