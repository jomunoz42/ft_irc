
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
	std::cout << "ARGS SIZE: " << args.size() << std::endl;
	for (size_t i = 0; i < args.size(); ++i)
		std::cout << "[" << i << "] = " << args[i] << std::endl;

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

// void Server::commandMode(Client &client, std::vector<std::string> &args) 
// {
	// MODE #channel +i
	// MODE #channel -i
	// MODE #channel +t
	// MODE #channel -t
	// MODE #channel +k password
	// MODE #channel -k
	// MODE #channel +o nick
	// MODE #channel -o nick
	// MODE #channel +l 10
	// MODE #channel -l

    // switch (new_mode)
    // {
    //     case 'i':
    //         if (!isInviteOnly())
    //             setInviteOnly(true);
    //         else
    //             setInviteOnly(false);
    //         return ;
    //     case 't':
    //         if (!isTopicRestricted())
    //             setTopicRestricted(true);
    //         else
    //             setTopicRestricted(false);
    //         return ;
    //     case 'k':
    //         if (!hasPassword())
    //         {
    //             setHasPassword(true);
    //             prompts user limit definition?
    //             setPassword(   );
    //         }
    //         else
    //             setHasPassword(false);
    //         return ;
    //     case 'o':


    //         if (! operator exists)
    //             addOperator
    //         else
    //             removeOperator
    //         return ;
    //     case 'l':
    //         if (!hasLimit())
    //         {
    //             setHasLimit(true);
    //             prompts user limit definition?
    //             setUserLimit(   );
    //         }
    //         else
    //             setHasLimit(false);
    //         return ;
    // }
// }











// KICK
// when kicking if channel has only 1 user that user should become operator?
// when kicking if chanel is already created but has no user that user 
// should become operator?
// if there is just a user on that channel he isn t chanel operator??