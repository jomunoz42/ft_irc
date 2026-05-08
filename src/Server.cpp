
#include "irc.hpp"

Server::Server(int port, std::string password, std::string name) : _port(port), _server_running(false), _password(password), \
 _server_name(name) {
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
	
}

Server::~Server(void) {}

Server::Server(const Server& other) : _port(other._port), _socket(other._socket), _server_running(other._server_running), \
_buffer(other._buffer), _password(other._password), _ip_address(other._ip_address), _server_name(other._server_name), \
_clients(other._clients), _signal_handler(other._signal_handler), _socket_list(other._socket_list), _errors(other._errors), \
_channels(other._channels), _server_commands(other._server_commands) {}

Server& Server::operator=(const Server& other) {
	if (this != &other) {
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
		this->_channels = other._channels;
		this->_server_commands = other._server_commands;
	}
	return (*this);
}

void Server::start(void) {
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0) {
		std::string error = std::strerror(errno);
		throw std::runtime_error(std::string("socket() failed: ") + error);
	}
	this->setNonBlocking(this->_socket);
	int opt = 1;
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::string error = std::strerror(errno);
		throw std::runtime_error(std::string("setsockopt() failed: ") + error);
	}
	this->_ip_address.sin_family = AF_INET;
	this->_ip_address.sin_port = htons(this->_port);
	this->_ip_address.sin_addr.s_addr = INADDR_ANY;
	if (bind(this->_socket, reinterpret_cast<sockaddr*>(&this->_ip_address), sizeof(this->_ip_address)) < 0) {
		std::string error = std::strerror(errno);
		throw std::runtime_error(std::string("bind() failed: ") + error);
	}
	if (listen(this->_socket, 100) < 0) {
		std::string error = std::strerror(errno);
		throw std::runtime_error(std::string("listen() failed: ") + error);
	}
	this->addSocket(this->_socket);
	this->_server_running = true;
	std::cout << "IRC Server is setup!" << std::endl;
}

void Server::run(void) {
	std::cout << "IRC Server is running!" << std::endl;
	while (this->_server_running) {
		if (poll(this->_socket_list.data(), this->_socket_list.size(), -1) < 0) {
			std::string error = std::strerror(errno);
			throw std::runtime_error(std::string("poll() failed: ") + error);
		}
		for (size_t i = 0; i < this->_socket_list.size();) {
			if (this->_socket_list.at(i).revents & POLLIN) {
				int socket = this->_socket_list.at(i).fd;

				if (socket == this->_socket) {
					this->addClient();
					++i;
					continue ;
				}
				else {
					std::map<int, Client>::iterator j = this->_clients.find(socket);
					if (j != this->_clients.end()) {
						Client &client = j->second;
						size_t size_before = this->_socket_list.size();
						e_data ret = this->receiveClientData(client);
						if (ret == RECEIVED) {
							std::cout << "IRC Server has received data!" << std::endl;
							this->processData(client);
						}
						else if (ret == DISCONNECTED || ret == ERROR)
							this->removeClient(client);
						if (this->_socket_list.size() < size_before)
							continue ;
					}
				}
			}
			++i;
		}
	}
}

void Server::stop(void) {
	this->_server_running = false;
	close(this->_socket);
	for (std::map<int, Client>::iterator i = this->_clients.begin(); i != this->_clients.end(); ++i) {
		close(i->second.getSocket());
	}
}

void Server::addSocket(int socket) {
	pollfd newSocket;
	newSocket.fd = socket;
	newSocket.events = POLLIN;
	newSocket.revents = 0;
	this->_socket_list.push_back(newSocket);
}

void Server::setNonBlocking(int socket) {
	int flags = fcntl(socket, F_GETFL, 0);
	if (flags < 0) {
		if (socket == this->_socket) {
			std::string error = std::strerror(errno);
			throw std::runtime_error(std::string("fcntl() failed: ") + error);
		}
		else {
			this->removeClient(this->_clients.at(socket));
			return ;
		}
	}
	if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) < 0) {
		if (socket == this->_socket) {
			std::string error = std::strerror(errno);
			throw std::runtime_error(std::string("fcntl() failed: ") + error);
		}
		else {
			this->removeClient(this->_clients.at(socket));
			return ;
		}
	}
}

void Server::addClient(void) {
	int client_socket = accept(this->_socket, NULL, NULL);
	if (client_socket < 0) {
		std::cerr << "accept() failed: " << std::strerror(errno) << std::endl;
		return ;
	}
	this->setNonBlocking(client_socket);
	this->addSocket(client_socket);
	this->_clients.insert(std::make_pair(client_socket, Client(client_socket)));
	std::cout << "Client Socket: " << client_socket << " has connected to IRC Server!" << std::endl;
}

void Server::removeClient(Client &client) {
	int client_socket = client.getSocket();
	std::vector<std::string> &channels = client.getChannels();
	for (std::vector<std::string>::iterator i = channels.begin(); i != channels.end(); ++i) {
		this->_channels.at(*i).removeUser(client);
		if (!this->_channels.at(*i).getUsers().size())
			this->_channels.erase(*i);
	}
	for (std::vector<pollfd>::iterator j = this->_socket_list.begin(); j != this->_socket_list.end(); ++j) {
		if (j->fd == client_socket) {
			this->_socket_list.erase(j);
			break ;
		}
	}
	std::cout << "Client Socket: " << client_socket << " has disconnected from IRC Server!" << std::endl;
	this->_clients.erase(client_socket);
	close(client_socket);
}

e_data Server::receiveClientData(Client &client) {
	char tmp[1024];
	int client_socket = client.getSocket();
	std::string &buffer = client.getRecvBuffer();
	while (true) {
		ssize_t bytes = recv(client_socket, tmp, sizeof(tmp), 0);
		if (bytes > 0) {
			buffer += std::string(tmp, bytes);
			return (RECEIVED);
		}
		if (bytes == 0)
			return (DISCONNECTED);
		if (errno == EINTR)
			continue;
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (RECEIVED);
		return (std::cerr << "recv() failed: " << std::strerror(errno) << std::endl, ERROR);
	}
}

void Server::processData(Client &client) {
	size_t pos;
	std::string &buffer = client.getRecvBuffer();
	while ((pos = buffer.find(DELIMITER)) != std::string::npos) {
		std::cout << "IRC Server is processing the data!" << std::endl;
		std::string line = buffer.substr(0, pos);
		buffer.erase(0, pos + 2);
		this->processCommand(client, line);
	}
}

void Server::processCommand(Client &client, std::string &line) {

	size_t pos = line.find(':');
	std::string message;
	if (pos != std::string::npos) {
		message = line.substr(pos + 1);
		line.erase(pos);
	}
	std::vector<std::string> args = split(line, " ");
	if (!message.empty())
		args.push_back(message);
	std::cout << "IRC Server is processing the command!" << std::endl;
	std::string command = args.at(0);
	// if (!client.isRegistered() && command != "PASS" && command != "NICK" && command != "USER") {}
	std::map<std::string, commandPtrServer>::iterator i = this->_server_commands.find(command);
	if (i == this->_server_commands.end())
		return (this->sendError(client, ERR_UNKNOWNCOMMAND, command));
	commandPtrServer command_executer = this->_server_commands.at(command);
	(this->*command_executer)(client, args);
}

void Server::sendMessage(Client &client, std::string &message) {
	size_t pos = message.find(DELIMITER);
	if (pos == std::string::npos)
		message += DELIMITER;
	/*size_t bytes = */send(client.getSocket(), message.c_str(), message.length(), 0);
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

void Server::commandPass(Client &client, std::vector<std::string> &args) {
	if (args.at(1) == this->_password)
		client.passwordAccepted();
	client.registerClient();
}

void Server::commandNick(Client &client, std::vector<std::string> &args) {
	// for (size_t i = 0; i < this->_clients.size(); ++i) {
	// 	if (args.at(1) == this->_clients.at(i).getNickname()) {}
	// 		//Implement sendError()
	// }
	client.setNickname(args.at(1));
	client.registerClient();
}

void Server::commandUser(Client &client, std::vector<std::string> &args) {
	client.setUsername(args.at(1));
	client.registerClient();
}

void Server::commandJoin(Client &client, std::vector<std::string> &args) {
	
}

void Server::commandPrivmsg(Client &client, std::vector<std::string> &args) {

}