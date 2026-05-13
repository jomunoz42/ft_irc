
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class Channel;

typedef void (Channel::*commandPtrChannel)(std::string);

class Channel 
{
	private:

		std::string _channel_name;
		std::string _channel_topic;
		std::string _channel_password;
		std::vector<Client*> _users;
		std::vector<Client*> _operators;
		std::map<std::string, commandPtrChannel> _channel_commands;

	public:
	
		Channel(std::string name);
		~Channel();
		Channel(const Channel& other);
		Channel& operator=(const Channel& other);
		void setTopic(const std::string new_topic);
		std::string getTopic(void) const;
		std::vector<Client*> getUsers(void);
		std::string getNameList(void) const;
		void addUser(Client &client);
		void removeUser(Client &client);
		void addOperator(Client &client);
		void removeOperator(Client &client);
		// void commandKick(std::string user);
		// void commandInvite(std::string user);
		// void commandTopic(std::string new_topic);
		// void commandMode(std::string new_mode);
};

#endif