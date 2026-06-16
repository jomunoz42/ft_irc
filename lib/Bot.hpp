#ifndef BOT_HPP
#define BOT_HPP

# include "Client.hpp"

class Bot : public Client
{
    private:
        int _port;
        int _socket;
        std::string _password;
        std::string _host;
        std::string _name;
        void logIn(void);
        void listen(void);

    public:
        Bot(int port, const std::string& password);
        Bot(const Bot& other);
        Bot& operator=(const Bot& other);
        ~Bot();

        void setPort(int port);
        void setPassword(const std::string& password);
        void setHost(const std::string& host);
        void setName(const std::string& host);

        std::string getHost(void) const;
        std::string getPassword(void) const;
        int getPort(void) const;
        std::string getMessage(std::string str);

        void run(void);
        bool isCommand(const std::string& message) const;
        void executeCommand(const std::string& command, const std::string& client) const;
};  

std::string getTime(void);
std::string getJoke(void);
std::string getInfo(void);
std::string listCommand(void);
int getIndex(const std::string& s);

#endif