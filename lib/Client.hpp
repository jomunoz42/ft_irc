
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
		bool _registered;
	public:
		Client(int socket);
		~Client();
		Client(const Client& other);
		Client& operator=(const Client& other);
		void setNickname(const std::string nickname);
		std::string getNickname(void) const;
		void setUsername(const std::string username);
		std::string getUsername(void) const;
		int getSocket(void) const;
		std::string &getRecvBuffer(void);
		std::string &getSendBuffer(void);
		std::vector<std::string> &getChannels(void);
		bool isRegistered(void) const;
		void passwordAccepted(void);
		void registerClient(void);
};

#endif