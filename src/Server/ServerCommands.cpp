
#include "irc.hpp"

void Server::commandPass(Client &client, std::vector<std::string> &args) 
{
	if (args.size() < 2)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	bool was_registered = client.isRegistered();
	if (args.at(1) == this->_password)
		client.passwordAccepted();
	else
		return (this->sendError(client, ERR_PASSWDMISMATCH, args.at(0)));
	if (!was_registered)	
		client.registerClient();
	if (!was_registered && client.isRegistered()) 
	{
		std::string message = "You are now registered to " + this->_server_name;
		this->sendMessage(client, message);
	}
}

void Server::commandNick(Client &client, std::vector<std::string> &args) 
{
	if (args.size() < 2)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	for (std::map<int, Client>::iterator i = this->_clients.begin(); i != this->_clients.end(); ++i) 
	{
		Client &search_client = i->second;
		if (args.at(1) == search_client.getNickname() && &client != &search_client)
			return (this->sendError(client, ERR_NICKNAMEINUSE, args.at(0)));
	}
	bool was_registered = client.isRegistered();
	client.setNickname(args.at(1));
	if (!was_registered)	
		client.registerClient();
	if (!was_registered && client.isRegistered()) 
	{
		std::string message = "You are now registered to " + this->_server_name;
		this->sendMessage(client, message);
	}
}

void Server::commandUser(Client &client, std::vector<std::string> &args) {
	if (args.size() < 2)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	bool was_registered = client.isRegistered();
	client.setUsername(args.at(1));
	if (!was_registered)	
		client.registerClient();
	if (!was_registered && client.isRegistered()) 
	{
		std::string message = "You are now registered to " + this->_server_name;
		this->sendMessage(client, message);
	}
}

void Server::commandJoin(Client &client, std::vector<std::string> &args) 
{
	std::cout << "ARGS SIZE: " << args.size() << std::endl;
	for (size_t i = 0; i < args.size(); ++i)
		std::cout << "[" << i << "] = " << args[i] << std::endl;

	if (args.size() < 2)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	if (!client.isRegistered())
		return (this->sendError(client, ERR_NOTREGISTERED, args.at(0)));

	std::string channelName = args[1];

	std::map<std::string, Channel>::iterator it = this->_channels.find(channelName);

	if (it == this->_channels.end())
	{
		this->_channels.insert(std::make_pair(channelName, Channel(channelName)));
		it = this->_channels.find(channelName);

		it->second.addOperator(client);

		std::cout << "Created channel: " << channelName << std::endl;
	}

	it->second.addUser(client);

	std::cout << client.getNickname()
			  << " joined "
			  << channelName
			  << std::endl;
}

void Server::commandPrivmsg(Client &client, std::vector<std::string> &args) 
{
	if (args.size() < 3)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	if (!client.isRegistered())
		return (this->sendError(client, ERR_NOTREGISTERED, args.at(0)));

	std::string target = args[1];
	std::string message = args[2];

	if (target.empty() || message.empty())
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));

	std::string fullMessage = ":" + client.getNickname()
		+ " PRIVMSG " + target + " :" + message + "\r\n";

	if (target[0] == '#')
	{
		std::map<std::string, Channel>::iterator channelIt = this->_channels.find(target);

		if (channelIt == this->_channels.end())
			return (this->sendError(client, ERR_NOSUCHCHANNEL, target));

		Channel &channel = channelIt->second;

		if (!channel.hasUser(client))
			return (this->sendError(client, ERR_NOTONCHANNEL, target));

		this->broadcastMessage(channel, fullMessage, &client);
	}
	else
	{
		std::map<int, Client>::iterator it = this->_clients.begin();

		while (it != this->_clients.end())
		{
			if (it->second.getNickname() == target)
			{
				this->sendMessage(it->second, fullMessage);
				return;
			}
			++it;
		}
		return (this->sendError(client, ERR_NOSUCHNICK, target));
	}
}

