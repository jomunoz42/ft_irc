
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

void Server::commandUser(Client &client, std::vector<std::string> &args) 
{
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
	if (args.size() < 2)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	if (!client.isRegistered())
		return (this->sendError(client, ERR_NOTREGISTERED, args.at(0)));

	std::string chName = args[1];

	std::map<std::string, Channel>::iterator it = this->_channels.find(chName);

	if (it == this->_channels.end())
	{
		this->_channels.insert(std::make_pair(chName, Channel(chName)));
		it = this->_channels.find(chName);

		it->second.addUser(client);
		it->second.addOperator(client);
		
		std::cout << "Created channel: " << chName << std::endl;
		std::cout << client.getNickname() << " joined " << chName << std::endl;
	}

	if (it->second.hasUser(client)) return ;

	if (it->second.hasLimit() && it->second.getUsers().size() >= it->second.getUserLimit())
		return (this->sendError(client, ERR_CHANNELISFULL, chName));

	it->second.addUser(client);

	std::cout << client.getNickname() << " joined " << chName << std::endl;
}

void Server::commandPrivmsg(Client &client, std::vector<std::string> &args) 
{
	if (args.size() < 3)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	if (!client.isRegistered())
		return (this->sendError(client, ERR_NOTREGISTERED, args.at(0)));

	std::string target = args[1], message = args[2];

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

void Server::commandKick(Client &client, std::vector<std::string> &args)
{	
	if (args.size() < 3)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	if (!client.isRegistered())
		return (this->sendError(client, ERR_NOTREGISTERED, args.at(0)));

	std::string chName = args[1], target = args[2], reason = "";
	if (args.size() >= 4) reason = " :" + args[3];

	std::map<std::string, Channel>::iterator channelIt = this->_channels.find(chName);

	if (channelIt == this->_channels.end())
		return (this->sendError(client, ERR_NOSUCHCHANNEL, chName));

	Channel &channel = channelIt->second;

	if (!channel.hasUser(client))
		return (this->sendError(client, ERR_NOTONCHANNEL, chName));
	if (!channel.hasOperator(client))
		return (this->sendError(client, ERR_CHANOPRIVSNEEDED, chName));

	std::map<int, Client>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		if (it->second.getNickname() == target)
			break ;
		++it;
	}

	if (it == this->_clients.end())
		return (this->sendError(client, ERR_NOSUCHNICK, target));
	if (!channel.hasUser(it->second))
		return (this->sendError(client, ERR_USERNOTINCHANNEL, target));

	std::string fullMessage = ":" + client.getNickname()
			+ " KICK " + chName + " "  + target + reason + "\r\n";

	this->broadcastMessage(channel, fullMessage, NULL);
	channel.removeUser(it->second);
	channel.removeOperator(it->second);
}

// void Server::commandInvite(Client &client, std::vector<std::string> &args) 
// {
	// INVITE nick #channel

    // 1. check registered
	// 2. check enough args
	// 3. find target nick
	// 4. find channel
	// 5. check inviter is in channel
	// 6. check inviter is operator
	// 7. mark target as invited
	// 8. send invite message to target
// }

void Server::commandTopic(Client &client, std::vector<std::string> &args) 
{
	if (args.size() < 2)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	if (!client.isRegistered())
		return (this->sendError(client, ERR_NOTREGISTERED, args.at(0)));

	std::string chName = args[1];

	std::map<std::string, Channel>::iterator channelIt = this->_channels.find(chName);

	if (channelIt == this->_channels.end())
		return (this->sendError(client, ERR_NOSUCHCHANNEL, chName));

	Channel &channel = channelIt->second;

	if (!channel.hasUser(client))
		return (this->sendError(client, ERR_NOTONCHANNEL, chName));

    if (args.size() == 2)
	{
		if (channel.getTopic().empty())
		{
			std::string msg = chName + " :No topic is set";
			return (this->sendReply(client, RPL_NOTOPIC, msg));
		}

		std::string topicMsg = chName + " :" + channel.getTopic();
		return (this->sendReply(client, RPL_TOPIC, topicMsg));
	}

	if (channel.isTopicRestricted() && !channel.hasOperator(client))
		return (this->sendError(client, ERR_CHANOPRIVSNEEDED, chName));

	std::string newTopic = args[2];
	
	channel.setTopic(newTopic);

	std::string fullMessage = ":" + client.getNickname()
		+ " TOPIC " + chName + " :" + newTopic + "\r\n";

	this->broadcastMessage(channel, fullMessage, NULL);
}

void Server::commandMode(Client &client, std::vector<std::string> &args) 
{
	std::cout << "ARGS SIZE: " << args.size() << std::endl;
	for (size_t i = 0; i < args.size(); ++i)
		std::cout << "[" << i << "] = " << args[i] << std::endl;

	if (args.size() < 3)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	if (!client.isRegistered())
		return (this->sendError(client, ERR_NOTREGISTERED, args.at(0)));
	
	std::string chName = args[1], mode = args[2], param = "";
	if (args.size() >= 4) param = args[3];

	std::map<std::string, Channel>::iterator channelIt = this->_channels.find(chName);
	if (channelIt == this->_channels.end())
		return (this->sendError(client, ERR_NOSUCHCHANNEL, chName));
	Channel &channel = channelIt->second;

	if (!channel.hasUser(client))
		return (this->sendError(client, ERR_NOTONCHANNEL, chName));
	if (!channel.hasOperator(client))
		return (this->sendError(client, ERR_CHANOPRIVSNEEDED, chName));
	
    if (mode.size() != 2 || (mode[0] != '+' && mode[0] != '-'))
		return (this->sendError(client, ERR_UNKNOWNMODE, mode));

	bool adding = (mode[0] == '+');
	char flag = mode[1];

	switch (flag)
	{
		case 'i':
			channel.setInviteOnly(adding);
			break;

		case 't':
			channel.setTopicRestricted(adding);
			break;

		case 'k':
			if (adding)
			{
				if (param.empty())
					return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
				channel.setHasPassword(true);
				channel.setPassword(param);
			}
			else
				channel.setHasPassword(false), channel.setPassword("");
			break;

		case 'l':
			if (adding)
			{
				if (param.empty())
					return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
				channel.setHasLimit(true);
				channel.setUserLimit(std::atoi(param.c_str()));
			}
			else
				channel.setHasLimit(false), channel.setUserLimit(0);
			break;

		case 'o':
		{
			if (param.empty())
				return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));

			std::map<int, Client>::iterator it = this->_clients.begin();
			while (it != this->_clients.end())
			{
				if (it->second.getNickname() == param)
					break;
				++it;
			}

			if (it == this->_clients.end())
				return (this->sendError(client, ERR_NOSUCHNICK, param));
			if (!channel.hasUser(it->second))
				return (this->sendError(client, ERR_USERNOTINCHANNEL, param));

			if (adding)
				channel.addOperator(it->second);
			else
				channel.removeOperator(it->second);
			break;
		}
		default:
			return (this->sendError(client, ERR_UNKNOWNMODE, mode));
	}

	std::string fullMessage = ":" + client.getNickname()
			+ " MODE " + chName + " " + mode;

	if (!param.empty())
		fullMessage += " " + param;
	fullMessage += "\r\n";

	this->broadcastMessage(channel, fullMessage, NULL);
}











// KICK
// when kicking if channel has only 1 user that user should become operator?
// when kicking if chanel is already created but has no user that user 
// should become operator?
// if there is just a user on that channel he isn t chanel operator??