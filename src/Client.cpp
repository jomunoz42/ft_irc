
#include "irc.hpp"

Client::Client(int socket) : _socket(socket) {}

Client::~Client() {}

Client::Client(const Client& other) : _socket(other._socket), _nickname(other._nickname), _username(other._username), \
_recv_buffer(other._recv_buffer), _send_buffer(other._send_buffer), _channels(other._channels), \
_pass_accepted(other._pass_accepted), _registered(other._registered) {}

Client& Client::operator=(const Client& other) {
	if (this != &other) {
		this->_socket = other._socket;
		this->_nickname = other._nickname;
		this->_username = other._username;
		this->_recv_buffer = other._recv_buffer;
		this->_send_buffer = other._send_buffer;
		this->_channels = other._channels;
		this->_pass_accepted = other._pass_accepted;
		this->_registered = other._registered;
	}
	return (*this);
}

void Client::setNickname(const std::string nickname) {
	if (!nickname.empty()) {
		std::cout << "Client socket: " << this->_socket << " nickname has been set to " << nickname << std::endl;
		this->_nickname = nickname;
	}
}

std::string Client::getNickname(void) const {
	return (this->_nickname);
}

void Client::setUsername(const std::string username) {
	if (!username.empty()) {
		std::cout << "Client socket: " << this->_socket << " username has been set to " << username << std::endl;
		this->_username = username;
	}
}

std::string Client::getUsername(void) const {
	return (this->_username);
}

int Client::getSocket(void) const {
	return (this->_socket);
}

std::string &Client::getRecvBuffer(void) {
	return (this->_recv_buffer);
}

std::string &Client::getSendBuffer(void) {
	return (this->_send_buffer);
}

std::vector<std::string> &Client::getChannels(void) {
	return (this->_channels);
}

bool Client::isRegistered(void) const {
	return (this->_registered);
}

void Client::passwordAccepted(void) {
	this->_pass_accepted = true;
}

void Client::registerClient(void) {
	if (this->_pass_accepted && !this->_nickname.empty() && !this->_username.empty()) {
		std::cout << "Client socket: " << this->_socket << " has been registered to IRC Server!" << std::endl;
		this->_registered = true;
	}
}