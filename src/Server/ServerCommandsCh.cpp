
#include "Server.hpp"

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
	it->second.removeChannel(chName);
	if (channel.getUsers().empty())
		this->_channels.erase(chName);
}

void Server::commandInvite(Client &client, std::vector<std::string> &args) 
{
	std::cout << "ARGS SIZE: " << args.size() << std::endl;
	for (size_t i = 0; i < args.size(); ++i)
		std::cout << "[" << i << "] = " << args[i] << std::endl;

	if (args.size() < 3)
		return (this->sendError(client, ERR_NEEDMOREPARAMS, args.at(0)));
	if (!client.isRegistered())
		return (this->sendError(client, ERR_NOTREGISTERED, args.at(0)));

	std::string target = args[1], chName = args[2];

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

	if (channel.hasUser(it->second))	return ;

	channel.addInvited(it->second);

	std::string message = ":" + client.getNickname()
		+ " INVITE " + target + " " + chName + "\r\n";

	this->sendMessage(it->second, message);
}

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
        std::string msg = ":" + this->_server_name + " 332 " + client.getNickname() 
                + " " + chName + " :" + channel.getTopic();
        return (this->sendMessage(client, msg));
	}

	if (args.size() >= 3 && channel.isTopicRestricted() && !channel.hasOperator(client))
		return (this->sendError(client, ERR_CHANOPRIVSNEEDED, chName));

	std::string newTopic = args[2];
	
	channel.setTopic(newTopic);

	std::string fullMessage = ":" + client.getNickname()
		+ " TOPIC " + chName + " :" + newTopic + "\r\n";

	this->broadcastMessage(channel, fullMessage, NULL);
}

void Server::commandMode(Client &client, std::vector<std::string> &args) 
{
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
// when kicking a use if channel ends up having only 1 
// user that user should become operator?