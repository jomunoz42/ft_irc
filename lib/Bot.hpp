#ifndef BOT_HPP
#define BOT_HPP

# include "Client.hpp"

class Bot {
    private:
        bool _channel;

    public:
        Bot();
        Bot(const Bot& other);
        Bot& operator=(const Bot& other);
        ~Bot();

        bool isCommand(const std::string& message) const;
        bool getChannel(void) const;
        std::string executeCommand(const std::string& command, const Client& client, const Channel* channel) const;
};  

std::string getTime(void);
std::string getJoke(void);
std::string getInfo(void);
std::string displayChannels(void);
int getIndex(const std::string& s);

#endif