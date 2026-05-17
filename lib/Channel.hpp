
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class Channel 
{
	private:

		std::string 			_channel_name;
		std::string 			_channel_topic;
		std::string 			_channel_password;
		std::vector<Client*> 	_users;
		std::vector<Client*> 	_operators;
		bool 					_invite_only;
		bool 					_topic_restricted;
		bool 					_has_password;
		bool 					_has_limit;
		size_t					_user_limit;

	public:
	
		Channel(std::string name);
		Channel(const Channel& other);
		Channel& operator=(const Channel& other);
		~Channel();
		
		bool				 isInviteOnly(void) const;
		bool				 isTopicRestricted(void) const;
		bool				 hasPassword(void) const;
		bool				 hasLimit(void) const;
		size_t				 getUserLimit(void) const;
		std::string 		 getNameList(void) const;
		std::string 		 getTopic(void) const;
		std::string 		 getPassword(void) const;
		std::vector<Client*> getUsers(void);
		
		void				 setInviteOnly(bool status);
		void				 setTopicRestricted(bool status);
		void				 setHasPassword(bool status);
		void				 setHasLimit(bool status);
		void				 setUserLimit(size_t limit);
		void 				 setTopic(const std::string new_topic);
		void 				 setPassword(const std::string &password);

		void 				 addUser(Client &client);
		void 				 removeUser(Client &client);
		bool 				 hasUser(Client &client);
		
		void 				 addOperator(Client &client);
		void 				 removeOperator(Client &client);
		bool 				 hasOperator(Client &client);
};

#endif