 
 #include "irc.hpp"

std::string Client::getNickname(void) const 
{
	return (this->_nickname);
}

std::string Client::getUsername(void) const 
{
	return (this->_username);
}

int Client::getSocket(void) const 
{
	return (this->_socket);
}

std::string &Client::getRecvBuffer(void) 
{
	return (this->_recv_buffer);
}

std::string &Client::getSendBuffer(void) 
{
	return (this->_send_buffer);
}

std::vector<std::string> &Client::getChannels(void) 
{
	return (this->_channels);
}

std::string Client::getPrefix(const std::string host) const 
{
	return (this->_nickname + "!" + this->_username + "@" + host);
}