
#include "irc.hpp"

Client::Client(int socket) : _socket(socket), _pass_accepted(false), _has_nickname(false), _has_username(false), _registered(false) {}

Client::~Client() {}

Client::Client(const Client& other) : _socket(other._socket), _nickname(other._nickname), _username(other._username), \
_recv_buffer(other._recv_buffer), _send_buffer(other._send_buffer), _channels(other._channels), \
_pass_accepted(other._pass_accepted), _has_nickname(other._has_nickname), _has_username(other._has_username), \
_registered(other._registered) {}

Client& Client::operator=(const Client& other) 
{
	if (this != &other) {
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