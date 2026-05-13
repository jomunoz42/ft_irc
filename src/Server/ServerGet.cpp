
#include "irc.hpp"

std::string Server::getPrefix(void) const 
{
	return (":" + this->_server_name);
}

pollfd &Server::getPollfd(const int socket) 
{
	for (size_t i = 0; i < this->_socket_list.size(); ++i) 
	{
		if (this->_socket_list.at(i).fd == socket)
			return (this->_socket_list.at(i));
	}
	throw std::runtime_error("pollfd not found");
}