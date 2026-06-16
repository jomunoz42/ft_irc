/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommands.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbongiov <pbongiov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 19:36:14 by pbongiov          #+#    #+#             */
/*   Updated: 2026/06/16 20:58:51 by pbongiov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"


void Bot::logIn(void)
{
    std::string passMsg("PASS " + _password + DELIMITER);
    std::string nickMsg("NICK " + _name + DELIMITER);
    std::string userMsg("USER " + _name + " 0 * :" + _name + DELIMITER);

    send(_socket, passMsg.c_str(), passMsg.size(), 0);
    send(_socket, nickMsg.c_str(), nickMsg.size(), 0);
    send(_socket, userMsg.c_str(), userMsg.size(), 0);
}

std::string getSender(std::string s)
{
    int n = s.find("PRIVMSG");
    
    return (s.substr(1, n - 2));
}

std::string Bot::getMessage(std::string s)
{
    std::string name(_name + " :");
    int n = s.find(name.c_str());

    return (s.substr(n + name.size(), sizeof(s.c_str())));
}

void Bot::listen(void)
{
    char buf[1025];
    std::string temp;

    while (1)
    {
        ssize_t n = recv(_socket, buf, sizeof(buf) - 1, 0);
        
        if (n < 0)
        {
            std::cerr << "recv() error" << std::endl;
            return;
        }
        if (n == 0)
        {
            std::cout << "Server closed connection" << std::endl;
            return;
        }
        buf[n] = '\0';
        temp += buf;
        size_t pos;
        while ((pos = temp.find("\r\n")) != std::string::npos)
        {
            std::string line = temp.substr(0, pos);
            temp.erase(0, pos + 2);

            std::cout << "RECV: " << line << std::endl;

            if (line.find("PRIVMSG") == std::string::npos)
                continue;

            std::string sender = getSender(buf);
            std::string message = getMessage(buf);
            
            std::cout << "SENDER: [" << sender << "]" << std::endl;
            std::cout << "MESSAGE: " << message << std::endl;

            if (this->isCommand(message))
                this->executeCommand(message, sender);
            else
            {
                std::string msg("PRIVMSG " + sender + " :Not a valid Command. Try !list to see all Commands\r\n");
                send(_socket, msg.c_str(), msg.size(), 0);
            }
        }
    }
}

void Bot::run(void)
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
    {
        std::cerr << "Error on socket()\n";
        return;
    }
    
    struct sockaddr_in s;
    s.sin_family = AF_INET;
    s.sin_port = htons(_port);
    inet_pton(AF_INET, _host.c_str(), &s.sin_addr);

    connect(_socket, (struct sockaddr *)&s, sizeof(s));
    
    this->logIn();
    this->listen();
}

bool Bot::isCommand(const std::string& message) const { return (message[0] == '!');}

void Bot::executeCommand(const std::string& command, const std::string& client) const
{
    std::string message("PRIVMSG " + client + " :");
    int index = getIndex(command);
    
    switch (index)
    {
        case 0:
            message += "Hello " + client + "!\r\n";
            break;

        case 1:
            message += "Current time is " + getTime() + ".\r\n";
            break;
            
        case 2:
            message += "Here are some useful information:\n" + getInfo() + "\r\n";
            break;
            
        case 3:
            message += getJoke() + "\r\n";
            break;
            
        case 4:
            message += listCommand() + "\r\n";
            break;
            
        default:
            message += "That's not a command :P. Try !list to see all Commands.\r\n";
    }

    std::cout << message << std::endl;
    
    send(_socket, message.c_str(), message.size(), 0);
}