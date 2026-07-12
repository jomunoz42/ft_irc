
#include "Server.hpp"

void Server::addSocket(int socket) 
{
	pollfd newSocket;
	newSocket.fd = socket;
	newSocket.events = POLLIN;
	newSocket.revents = 0;
	this->_socket_list.push_back(newSocket);
}

void Server::setNonBlocking(int socket) 
{
	if (fcntl(socket, F_SETFL, O_NONBLOCK) < 0) 
	{
		if (socket == this->_socket) 
		{
			std::string error = std::strerror(errno);
			throw std::runtime_error(std::string("fcntl() failed: ") + error);
		}
		else 
		{
			this->removeClient(this->_clients.at(socket));
			return ;
		}
	}
}