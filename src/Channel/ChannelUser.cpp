
#include "irc.hpp"

void Channel::addUser(Client &client) 
{
	if (this->hasUser(client))
		return ;

	this->_users.push_back(&client);

	std::cout << "User " 
			  << client.getNickname() 
			  << " has been added to " 
			  << this->_channel_name 
			  << std::endl;
}

void Channel::removeUser(Client &client) 
{
	for (std::vector<Client*>::iterator i = this->_users.begin(); i != this->_users.end(); ++i) 
	{
		if (*i == &client) 
		{
			this->_users.erase(i);
			std::cout << "User " << client.getNickname() << " has been removed from " << this->_channel_name << std::endl;
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
	if (this->hasOperator(client))
		return ;

	this->_operators.push_back(&client);

	std::cout << "User " 
			  << client.getNickname() 
			  << " is now an operator in " 
			  << this->_channel_name 
			  << std::endl;
}

void Channel::removeOperator(Client &client) 
{
	for (std::vector<Client*>::iterator j = this->_operators.begin(); j != this->_operators.end(); ++j) 
	{
		if (*j == &client) 
		{
			this->_operators.erase(j);
			std::cout << "User " << client.getNickname() << " is no longer an operator in " << this->_channel_name << std::endl;
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