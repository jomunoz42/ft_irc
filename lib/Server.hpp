
#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"

class Server;
class Client;
class Channel;

typedef enum e_data 
{
	SUCCESS,
	DISCONNECTED,
	ERROR
}	e_data;

typedef void (Server::*commandPtrServer)(Client &, std::vector<std::string> &);

class Server 
{
	private:

		int 					_port;
		int 					_socket;
		bool					_server_running;
		std::string 			_buffer;
		std::string 			_password;
		sockaddr_in 			_ip_address;
		std::string 			_server_name;
		struct sigaction 				_signal_handler;
		std::vector<pollfd> 			_socket_list;
		std::map<int, Client>   		_clients;
		std::map<int, std::string> 		_errors;
		std::map<int, std::string> 		_replies;
		std::map<std::string, Channel>  		_channels;
		std::map<std::string, commandPtrServer> _server_commands;

	public:

		Server(int port, std::string password, std::string name);
		Server(const Server& other);
		Server& operator=(const Server& other);
		~Server();

		void 		start(void);
		void 		run(void);
		void 		stop(void);
		void 		addClient(void);
		void 		addSocket(int socket);

		void 		setNonBlocking(int socket);
		void 		processData(Client &client);
		void 		removeClient(Client &client);
		e_data 		flushSendBuffer(Client &client);
		e_data 		receiveClientData(Client &client);
		void 		processCommand(Client &client, std::string &line);

		void 		broadcastMessage(Channel &channel, std::string &message, Client *exclude);
		void 		sendMessage(Client &client, std::string &message);
		void 		sendError(Client &client, int code, std::string &command);
		void 		sendReply(Client &client, int code, std::string &message);
		
		void 		commandPass(Client &client, std::vector<std::string> &args);
		void 		commandNick(Client &client, std::vector<std::string> &args);
		void 		commandUser(Client &client, std::vector<std::string> &args);
		void 		commandJoin(Client &client, std::vector<std::string> &args);
		void 		commandPrivmsg(Client &client, std::vector<std::string> &args);

		void 		commandKick(Client &client, std::vector<std::string> &args);
		void 		commandInvite(Client &client, std::vector<std::string> &args);
		void 		commandTopic(Client &client, std::vector<std::string> &args);
		void 		commandMode(Client &client, std::vector<std::string> &args);

		std::string getPrefix(void) const;
		pollfd&		getPollfd(const int socket);
};

#endif