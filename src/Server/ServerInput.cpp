
#include "irc.hpp"

e_data Server::receiveClientData(Client &client) 
{
	char tmp[1024];
	int client_socket = client.getSocket();
	std::string &buffer = client.getRecvBuffer();
	std::cout << "IRC Server has received data!" << std::endl;
	while (true) 
	{
		ssize_t bytes = recv(client_socket, tmp, sizeof(tmp), 0);
		if (bytes > 0) 
		{
			buffer += std::string(tmp, bytes);
			return (SUCCESS);
		}
		if (bytes == 0)
			return (DISCONNECTED);
		if (errno == EINTR)
			continue;
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (SUCCESS);
		return (std::cerr << "recv() failed: " << std::strerror(errno) << std::endl, ERROR);
	}
}

void Server::processData(Client &client) 
{
	size_t pos;
	std::string &buffer = client.getRecvBuffer();
	while ((pos = buffer.find(DELIMITER)) != std::string::npos) 
	{
		std::cout << "IRC Server is processing the data!" << std::endl;
		std::string line = buffer.substr(0, pos);
		buffer.erase(0, pos + 2);
		this->processCommand(client, line);
	}
}

void Server::processCommand(Client &client, std::string &line) 
{
	size_t pos = line.find(':');
	std::string message;
	if (pos != std::string::npos) 
	{
		message = line.substr(pos + 1);
		line.erase(pos);
	}
	std::vector<std::string> args = split(line, " ");
	if (!message.empty())
		args.push_back(message);
	std::cout << "IRC Server is processing the command!" << std::endl;
	std::string command = args.at(0);
	if (args.empty())
		return;
	std::map<std::string, commandPtrServer>::iterator i = this->_server_commands.find(command);
	if (i == this->_server_commands.end())
		return (this->sendError(client, ERR_UNKNOWNCOMMAND, command));
	commandPtrServer command_executer = this->_server_commands.at(command);
	(this->*command_executer)(client, args);
}