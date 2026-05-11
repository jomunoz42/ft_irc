
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"

class Client {
	private:
		int _socket;
		std::string _nickname;
		std::string _username;
		std::string _recv_buffer;
		std::string _send_buffer;
		std::vector<std::string> _channels;
		bool _pass_accepted;
		bool _has_nickname;
		bool _has_username;
		bool _registered;
	public:
		Client(int socket);
		~Client();
		Client(const Client& other);
		Client& operator=(const Client& other);
		void registerClient(void);
		int getSocket(void) const;
		void passwordAccepted(void);
		bool isRegistered(void) const;
		std::string &getSendBuffer(void);
		std::string &getRecvBuffer(void);
		std::string getUsername(void) const;
		std::string getNickname(void) const;
		void setUsername(const std::string username);
		std::vector<std::string> &getChannels(void);
		void setNickname(const std::string nickname);
		std::string getPrefix(const std::string host) const;
};

#endif