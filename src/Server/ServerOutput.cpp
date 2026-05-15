
#include "irc.hpp"

void Server::sendMessage(Client &client, std::string &message) 
{
	if (message.find(DELIMITER) == std::string::npos)
		message += DELIMITER;
	ssize_t bytes = send(client.getSocket(), message.c_str(), message.size(), 0);
	if (bytes < 0) 
	{
		if (errno == EWOULDBLOCK) 
		{
			client.getSendBuffer() += message;
			this->getPollfd(client.getSocket()).events |= POLLOUT;
		}
		else
			this->removeClient(client);
	}
	else if (static_cast<size_t>(bytes) < message.size()) 
	{
		client.getSendBuffer() += message.substr(bytes);
		this->getPollfd(client.getSocket()).events |= POLLOUT;
	}
}

e_data Server::flushSendBuffer(Client &client) 
{
	std::string &send_buffer = client.getSendBuffer();
	ssize_t bytes = send(client.getSocket(), send_buffer.c_str(), send_buffer.size(), 0);
	if (bytes > 0) 
	{
		send_buffer.erase(0, bytes);
		if (send_buffer.empty())
			this->getPollfd(client.getSocket()).events &= ~POLLOUT;
		return (SUCCESS);
	}
	if (bytes == 0)
		return (DISCONNECTED);
	if (errno == EWOULDBLOCK)
		return (SUCCESS);
	return (std::cerr << "send() failed: " << std::strerror(errno) << std::endl, ERROR);
}

void Server::sendError(Client &client, int code, std::string &command) 
{
	std::stringstream build_message;
	build_message << ":" << this->_server_name << " " << code << " " << client.getNickname() << " " << command << ": " << this->_errors.at(code);
	std::string message = build_message.str();
	this->sendMessage(client, message);
}

void Server::sendReply(Client &client, int code, std::string &message) 
{
	std::stringstream build_message;
	std::string reply_text;
	if (!message.empty())
		reply_text = message;
	else if (this->_replies.find(code) != this->_replies.end())
		reply_text = this->_replies.at(code);
	build_message << this->getPrefix() << " " << code << " " << client.getNickname();
	if (!reply_text.empty())
		build_message << " :" << reply_text;
	std::string built = build_message.str();
	this->sendMessage(client, built);
}

void Server::broadcastMessage(Channel &channel, std::string &message, Client *exclude) 
{
	std::vector<Client*> users = channel.getUsers();
	for (size_t i = 0; i < users.size() ; ++i) 
	{
		if (users.at(i) != exclude)
			this->sendMessage(*(users.at(i)), message);
	}
}