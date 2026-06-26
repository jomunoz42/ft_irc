
#include "Client.hpp"

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
		this->_nickname = nickname;
		this->_has_nickname = true;
	}
}

void Client::setUsername(const std::string username) 
{
	if (!username.empty()) 
	{
		this->_username = username;
		this->_has_username = true;
	}
}

void	Client::setSender(Client& target, std::string message)
{
	int listenSock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
    addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(0);

	bind(listenSock, (sockaddr *)&addr, sizeof(addr));
	listen(listenSock, 1);

	int port;
	socklen_t len = sizeof(addr);

	getsockname(listenSock, (sockaddr *)&addr, &len);
	port = ntohs(addr.sin_port);

	char ip[INET_ADDRSTRLEN];

	getsockname(_socket, (sockaddr *)&addr, &len);
	inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
	
	std::string temp(ip);
	std::stringstream s;

	s << port;
	message += " " + temp + " ";
	message += s.str();
	message += DELIMITER;

	send(target._socket, message.c_str(), message.size(), 0);
	std::cout << "MY PORT: " << port << "\nMY IP:" << temp << std::endl;
	
	int sendSock = accept(listenSock, NULL, NULL);
}

void Client::setReceiver(std::string message)
{
	std::vector<std::string> args = split(message, " ");
	int readSock = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(std::atoi(args[4].c_str()));
	inet_pton(AF_INET, args[3].c_str(), &addr.sin_addr);

	connect(readSock, (sockaddr *)&addr, sizeof(addr));
	std::cout << "CONNECTED TO PORT: " << args[4] << "\nCONNECTED TO IP:" << args[3] << std::endl;
}

void	Client::directConnection(Client& target, const std::string& message)
{
	this->setSender(target, message);
}
