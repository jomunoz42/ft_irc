
#include "irc.hpp"

Server::Server(int port, std::string password, std::string name) : _port(port), _server_running(false), _password(password), \
 _server_name(name) 
{
	std::memset(&this->_ip_address, 0, sizeof(this->_ip_address));
	this->_server_commands.insert(std::make_pair("PASS", &Server::commandPass));
	this->_server_commands.insert(std::make_pair("NICK", &Server::commandNick));
	this->_server_commands.insert(std::make_pair("USER", &Server::commandUser));
	this->_server_commands.insert(std::make_pair("JOIN", &Server::commandJoin));
	this->_server_commands.insert(std::make_pair("PRIVMSG", &Server::commandPrivmsg));
	this->_server_commands.insert(std::make_pair("KICK", &Server::commandKick));
	// this->_server_commands.insert(std::make_pair("INVITE", &Server::commandInvite));
	this->_server_commands.insert(std::make_pair("TOPIC", &Server::commandTopic));
	// this->_server_commands.insert(std::make_pair("MODE", &Server::commandMode));
	this->_errors.insert(std::make_pair(ERR_UNKNOWNCOMMAND, "Unknown command"));
	this->_errors.insert(std::make_pair(ERR_ERRONEUSNICKNAME, "Erroneous nickname"));
	this->_errors.insert(std::make_pair(ERR_NICKNAMEINUSE, "Nickname is already in use"));
	this->_errors.insert(std::make_pair(ERR_NOTREGISTERED, "You have not registered"));
	this->_errors.insert(std::make_pair(ERR_NEEDMOREPARAMS, "Not enough parameters"));
	this->_errors.insert(std::make_pair(ERR_PASSWDMISMATCH, "Password incorrect"));
	this->_errors.insert(std::make_pair(ERR_NOSUCHNICK, "No such nick"));
	this->_errors.insert(std::make_pair(ERR_NOSUCHCHANNEL, "No such channel"));
	this->_errors.insert(std::make_pair(ERR_NOTONCHANNEL, "You're not on that channel"));
	this->_errors.insert(std::make_pair(ERR_USERNOTINCHANNEL, "User not on that channel"));
	this->_errors.insert(std::make_pair(ERR_CHANOPRIVSNEEDED, "You're not channel operator"));
	this->_replies.insert(std::make_pair(RPL_WELCOME, "Welcome to the IRC Network"));
	this->_replies.insert(std::make_pair(RPL_YOURHOST, "Your host is " + this->_server_name));
	this->_replies.insert(std::make_pair(RPL_NOTOPIC, "No topic is set"));
	this->_replies.insert(std::make_pair(RPL_ENDOFNAMES, "End of /NAMES list"));
}

Server::Server(const Server& other) 
		: _port(other._port), 
		  _socket(other._socket), 
		  _server_running(other._server_running),
		  _buffer(other._buffer), 
		  _password(other._password), 
		  _ip_address(other._ip_address), 
		  _server_name(other._server_name),
		  _signal_handler(other._signal_handler),
		  _socket_list(other._socket_list), 
		  _clients(other._clients), 
		  _errors(other._errors),
		  _replies(other._replies), 
		  _channels(other._channels), 
		  _server_commands(other._server_commands)
{}

Server& Server::operator=(const Server& other) 
{
	if (this != &other) 
	{
		this->_port = other._port;
		this->_socket = other._socket;
		this->_server_running = other._server_running;
		this->_buffer = other._buffer;
		this->_password = other._password;
		this->_ip_address = other._ip_address;
		this->_server_name = other._server_name;
		this->_clients = other._clients;
		this->_signal_handler = other._signal_handler;
		this->_socket_list = other._socket_list;
		this->_errors = other._errors;
		this->_replies = other._replies;
		this->_channels = other._channels;
		this->_server_commands = other._server_commands;
	}
	return (*this);
}

Server::~Server(void) {}

std::string Server::getPrefix(void) const {return (":" + this->_server_name);}

pollfd &Server::getPollfd(const int socket) 
{
	for (size_t i = 0; i < this->_socket_list.size(); ++i) 
	{
		if (this->_socket_list.at(i).fd == socket)
			return (this->_socket_list.at(i));
	}
	throw std::runtime_error("pollfd not found");
}