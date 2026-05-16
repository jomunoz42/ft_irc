
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
	this->_errors.insert(std::make_pair(ERR_UNKNOWNCOMMAND, "Unknown command"));
	this->_errors.insert(std::make_pair(ERR_ERRONEUSNICKNAME, "Erroneous nickname"));
	this->_errors.insert(std::make_pair(ERR_NICKNAMEINUSE, "Nickname is already in use"));
	this->_errors.insert(std::make_pair(ERR_NOTREGISTERED, "You have not registered"));
	this->_errors.insert(std::make_pair(ERR_NEEDMOREPARAMS, "Not enough parameters"));
	this->_errors.insert(std::make_pair(ERR_PASSWDMISMATCH, "Password incorrect"));
	this->_replies.insert(std::make_pair(RPL_WELCOME, "Welcome to the IRC Network"));
	this->_replies.insert(std::make_pair(RPL_YOURHOST, "Your host is " + this->_server_name));
	this->_replies.insert(std::make_pair(RPL_NOTOPIC, "No topic is set"));
	this->_replies.insert(std::make_pair(RPL_ENDOFNAMES, "End of /NAMES list"));
	this->_errors.insert(std::make_pair(ERR_NOSUCHNICK, "No such nick"));
	this->_errors.insert(std::make_pair(ERR_NOSUCHCHANNEL, "No such channel"));
	this->_errors.insert(std::make_pair(ERR_NOTONCHANNEL, "You're not on that channel"));
	
}

Server::~Server(void) {}

Server::Server(const Server& other) : _port(other._port), _socket(other._socket), _server_running(other._server_running), \
_buffer(other._buffer), _password(other._password), _ip_address(other._ip_address), _server_name(other._server_name), \
_clients(other._clients), _signal_handler(other._signal_handler), _socket_list(other._socket_list), _errors(other._errors), \
_replies(other._replies), _channels(other._channels), _server_commands(other._server_commands) {}

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