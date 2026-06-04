#ifndef BOT_HPP
#define BOT_HPP

# include "Client.hpp"

class Bot : public Client
{
    private:
        int _port;
        std::string _password;
        void logIn(void);

    public:
        Bot(int port, const std::string& password);
        Bot(const Bot& other);
        Bot& operator=(const Bot& other);
        ~Bot();

        bool isCommand(const std::string& message) const;
        std::string executeCommand(const std::string& command, const Client& client) const;
};  

std::string getTime(void);
std::string getJoke(void);
std::string getInfo(void);
std::string displayChannels(void);
int getIndex(const std::string& s);

#endif