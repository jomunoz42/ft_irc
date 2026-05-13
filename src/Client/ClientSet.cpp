
#include "irc.hpp"

void Client::setNickname(const std::string nickname) 
{
	if (!nickname.empty()) 
	{
		std::cout << "Client socket: " << this->_socket << " nickname has been set to " << nickname << std::endl;
		this->_nickname = nickname;
		this->_has_nickname = true;
	}
}

void Client::setUsername(const std::string username) 
{
	if (!username.empty()) 
	{
		std::cout << "Client socket: " << this->_socket << " username has been set to " << username << std::endl;
		this->_username = username;
		this->_has_username = true;
	}
}