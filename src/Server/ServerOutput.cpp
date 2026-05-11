
#include "irc.hpp"

void Server::sendMessage(Client &client, std::string &message) {
	size_t pos = message.find(DELIMITER);
	if (pos == std::string::npos)
		message += DELIMITER;
	ssize_t bytes = send(client.getSocket(), message.c_str(), message.size(), 0);
	if (bytes < 0) {
		if (errno == EWOULDBLOCK) {
			client.getSendBuffer() += message;
			this->getPollfd(client.getSocket()).events |= POLLOUT;
		}
		else
			this->removeClient(client);
	}
	else if (static_cast<size_t>(bytes) < message.size()) {
		client.getSendBuffer() += message.substr(bytes);
		this->getPollfd(client.getSocket()).events |= POLLOUT;
	}
}

void Server::flushSendBuffer(Client &client) {
	std::string send_buffer = client.getSendBuffer();
	ssize_t bytes = send(client.getSocket(), send_buffer.c_str(), send_buffer.size(), 0);
	if (bytes > 0) {
		send_buffer.erase(0, bytes);
		if (send_buffer.empty())
			this->getPollfd(client.getSocket()).events &= ~POLLOUT;
	}
	if (bytes < 0) {
		if (errno == EWOULDBLOCK)
			return ;
		else
			this->removeClient(client);
	}
}

void Server::sendError(Client &client, int code, std::string &command) {
	std::stringstream build_message;
	build_message << ":" << this->_server_name << " " << code << " " << client.getNickname() << " " << command << ": " << this->_errors.at(code);
	std::string message = build_message.str();
	this->sendMessage(client, message);
}

void Server::broadcastMessage(Channel &channel, std::string &message, Client *exclude) {
	std::vector<Client*> users = channel.getUsers();
	for (size_t i = 0; i < users.size() ; ++i) {
		if (users.at(i) != exclude)
			this->sendMessage(*(users.at(i)), message);
	}
}