
#include "irc.hpp"

void Channel::addUser(Client &client) 
{
	this->_users.push_back(&client);
	std::cout << "User " << client.getNickname() << " has been added to " << this->_channel_name << std::endl;
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

void Channel::addOperator(Client &client) 
{
	this->_operators.push_back(&client);
	std::cout << "User " << client.getNickname() << " is now an operator in " << this->_channel_name << std::endl;
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