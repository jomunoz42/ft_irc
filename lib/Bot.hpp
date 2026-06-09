#ifndef BOT_HPP
#define BOT_HPP

# include "Client.hpp"

class Bot : public Client
{
    private:
        int _port;
        std::string _password;
        std::string _host;
        void logIn(void);
        bool sendAll(int socket_fd, const std::string& data);

    public:
        Bot(void);
        Bot(int port, const std::string& password);
        Bot(const Bot& other);
        Bot& operator=(const Bot& other);
        ~Bot();

        void setPort(int port);
        void setPassword(const std::string& password);
        void setHost(const std::string& host);

        std::string getHost(void) const;
        std::string getPassword(void) const;
        int getPort(void) const;
        bool sendLine(int socket_fd, const std::string& line);
        int connectToServer(void);
        void receiveResponse(int socket_fd);
        bool isCommand(const std::string& message) const;
        std::string executeCommand(const std::string& command, const Client& client) const;
};  

std::string getTime(void);
std::string getJoke(void);
std::string getInfo(void);
std::string displayChannels(void);
int getIndex(const std::string& s);

#endif