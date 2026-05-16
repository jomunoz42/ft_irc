
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