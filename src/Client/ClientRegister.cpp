
#include "irc.hpp"

bool Client::isRegistered(void) const {return (this->_registered);}

void Client::passwordAccepted(void) 
{
	std::cout << "Client socket: " 
			  << this->_socket 
			  << " has passed the correct password to IRC Server!" 
			  << std::endl;

	this->_pass_accepted = true;
}

void Client::registerClient(void) 
{
	if (this->_pass_accepted && this->_has_nickname && this->_has_username) 
	{
		std::cout << "Client socket: " 
				  << this->_socket 
				  << " has been registered to IRC Server!" 
				  << std::endl;

		this->_registered = true;
	}
}

void Client::addChannel(const std::string &channel)
{
	if (!this->hasChannel(channel))
		this->_channels.push_back(channel);
}

void Client::removeChannel(const std::string &channel)
{
	for (std::vector<std::string>::iterator it = this->_channels.begin();
		it != this->_channels.end(); ++it)
	{
		if (*it == channel)
		{
			this->_channels.erase(it);
			break;
		}
	}
}

bool Client::hasChannel(const std::string &channel)
{
	for (size_t i = 0; i < this->_channels.size(); ++i)
	{
		if (this->_channels[i] == channel)
			return (true);
	}
	return (false);
}