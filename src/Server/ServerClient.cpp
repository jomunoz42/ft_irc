
#include "irc.hpp"

void Server::addClient(void) 
{
	int client_socket = accept(this->_socket, NULL, NULL);
	if (client_socket < 0) 
	{
		std::cerr << "accept() failed: " << std::strerror(errno) << std::endl;
		return ;
	}
	this->setNonBlocking(client_socket);
	this->addSocket(client_socket);
	this->_clients.insert(std::make_pair(client_socket, Client(client_socket)));
	std::cout << "Client Socket: " << client_socket << " has connected to IRC Server!" << std::endl;
}

void Server::removeClient(Client &client)
{
	int client_socket = client.getSocket();
	std::vector<std::string> &channels = client.getChannels();
	for (std::vector<std::string>::iterator i = channels.begin(); i != channels.end(); ++i) 
	{
		this->_channels.at(*i).removeUser(client);
		this->_channels.at(*i).removeOperator(client);
		if (!this->_channels.at(*i).getUsers().size())
			this->_channels.erase(*i);
	}
	for (std::vector<pollfd>::iterator j = this->_socket_list.begin(); j != this->_socket_list.end(); ++j) 
	{
		if (j->fd == client_socket) 
		{
			this->_socket_list.erase(j);
			break ;
		}
	}
	std::cout << "Client Socket: " << client_socket << " has disconnected from IRC Server!" << std::endl;
	this->_clients.erase(client_socket);
	close(client_socket);
}