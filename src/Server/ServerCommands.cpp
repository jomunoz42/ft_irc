
#include "Server.hpp"
#include <cctype>

namespace
{
	bool isValidNickname(const std::string &nickname)
	{
		const std::string special = "[]\\`_^{|}";

		if (nickname.empty() || nickname.size() > 9)
			return (false);
		if (!std::isalpha(static_cast<unsigned char>(nickname[0]))
			&& special.find(nickname[0]) == std::string::npos)
			return (false);
		for (size_t i = 1; i < nickname.size(); ++i)
		{
			unsigned char current = static_cast<unsigned char>(nickname[i]);
			if (!std::isalnum(current) && special.find(nickname[i]) == std::string::npos
				&& nickname[i] != '-')
				return (false);
		}
		return (true);
	}

	void sendWelcomeReplies(Server &server, Client &client)
	{
		std::string welcome = "Welcome to the IRC Network";
		std::string host = "Your host is " + server.getPrefix().substr(1);
		server.sendReply(client, RPL_WELCOME, welcome);
		server.sendReply(client, RPL_YOURHOST, host);
	}
}

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
		sendWelcomeReplies(*this, client);
}

void Server::commandNick(Client &client, std::vector<std::string> &args) 
{
	if (args.size() < 2)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	if (!isValidNickname(args.at(1)))
		return (this->sendError(client, ERR_ERRONEUSNICKNAME, args.at(0)));
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
		sendWelcomeReplies(*this, client);
}

void Server::commandUser(Client &client, std::vector<std::string> &args) 
{
	if (args.size() < 5)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));

	bool was_registered = client.isRegistered();
	client.setUsername(args.at(1));

	if (!was_registered)	
		client.registerClient();
	if (!was_registered && client.isRegistered()) 
		sendWelcomeReplies(*this, client);
}

void Server::commandJoin(Client &client, std::vector<std::string> &args) 
{
	if (args.size() < 2)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	if (!client.isRegistered())
		return (this->sendError(client, ERR_NOTREGISTERED, args.at(0)));

	std::string chName = args[1], key = "";
	if (args.size() >= 3)
		key = args[2];

	std::map<std::string, Channel>::iterator it = this->_channels.find(chName);
	bool newChannel = false;

	if (it == this->_channels.end())
	{
		this->_channels.insert(std::make_pair(chName, Channel(chName)));
		it = this->_channels.find(chName);
		newChannel = true;
	}

	if (it->second.hasUser(client))
		return ;

	if (it->second.hasPassword())
	{
		if (key.empty() || key != it->second.getPassword())
			return (this->sendError(client, ERR_BADCHANNELKEY, chName));
	}

	if (it->second.hasLimit() && it->second.getUsers().size() >= it->second.getUserLimit())
		return (this->sendError(client, ERR_CHANNELISFULL, chName));

	if (it->second.isInviteOnly() && !it->second.hasInvited(client))
		return (this->sendError(client, ERR_INVITEONLYCHAN, chName));

	it->second.addUser(client);
	client.addChannel(chName);
	if (newChannel)
		it->second.addOperator(client);
	
	it->second.removeInvited(client);

	std::string joinMessage = ":" + client.getNickname() + " JOIN " + chName + "\r\n";
	this->broadcastMessage(it->second, joinMessage, NULL);

	if (it->second.getTopic().empty())
	{
		std::stringstream topicMessage;
		topicMessage << this->getPrefix() << " " << RPL_NOTOPIC << " " << client.getNickname()
			<< " " << chName << " :No topic is set";
		std::string builtTopic = topicMessage.str();
		this->sendMessage(client, builtTopic);
	}
	else
	{
		std::stringstream topicMessage;
		topicMessage << this->getPrefix() << " " << RPL_TOPIC << " " << client.getNickname()
			<< " " << chName << " :" << it->second.getTopic();
		std::string builtTopic = topicMessage.str();
		this->sendMessage(client, builtTopic);
	}

	std::vector<Client*> users = it->second.getUsers();
	std::stringstream names;
	for (size_t i = 0; i < users.size(); ++i)
	{
		if (i)
			names << ' ';
		if (it->second.hasOperator(*users.at(i)))
			names << '@';
		names << users.at(i)->getNickname();
	}

	std::stringstream namesReply;
	namesReply << this->getPrefix() << " " << RPL_NAMREPLY << " " << client.getNickname()
		<< " = " << chName << " :" << names.str();
	std::string builtNames = namesReply.str();
	this->sendMessage(client, builtNames);

	std::stringstream endReply;
	endReply << this->getPrefix() << " " << RPL_ENDOFNAMES << " " << client.getNickname()
		<< " " << chName << " :End of /NAMES list";
	std::string builtEnd = endReply.str();
	this->sendMessage(client, builtEnd);
}

void Server::commandPrivmsg(Client &client, std::vector<std::string> &args) 
{
	if (args.size() < 3)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	if (!client.isRegistered())
		return (this->sendError(client, ERR_NOTREGISTERED, args.at(0)));

	std::string target = args[1], message = args[2];
	for (size_t i = 3; i < args.size(); ++i)
		message += " " + args[i];

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
