
#include "irc.hpp"

Client::Client(int socket) : _socket(socket), _pass_accepted(false), _has_nickname(false), _has_username(false), _registered(false) {}

Client::~Client() {}

Client::Client(const Client& other) : _socket(other._socket), _nickname(other._nickname), _username(other._username), \
_recv_buffer(other._recv_buffer), _send_buffer(other._send_buffer), _channels(other._channels), \
_pass_accepted(other._pass_accepted), _has_nickname(other._has_nickname), _has_username(other._has_username), \
_registered(other._registered) {}

Client& Client::operator=(const Client& other) 
{
	if (this != &other) 
	{
		this->_socket = other._socket;
		this->_nickname = other._nickname;
		this->_username = other._username;
		this->_recv_buffer = other._recv_buffer;
		this->_send_buffer = other._send_buffer;
		this->_channels = other._channels;
		this->_pass_accepted = other._pass_accepted;
		this->_has_nickname = other._has_nickname;
		this->_has_username = other._has_username;
		this->_registered = other._registered;
	}
	return (*this);
}

std::string Client::getNickname(void) const {return (this->_nickname);}

std::string Client::getUsername(void) const {return (this->_username);}

int Client::getSocket(void) const {return (this->_socket);}

std::string &Client::getRecvBuffer(void) {return (this->_recv_buffer);}

std::string &Client::getSendBuffer(void) {return (this->_send_buffer);}

std::vector<std::string> &Client::getChannels(void) {return (this->_channels);}

std::string Client::getPrefix(const std::string host) const 
{
	return (this->_nickname + "!" + this->_username + "@" + host);
}

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