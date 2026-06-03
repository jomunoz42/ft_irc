/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbongiov <pbongiov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:23:54 by pbongiov          #+#    #+#             */
/*   Updated: 2026/06/03 23:01:20 by pbongiov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Channel.hpp"

Bot::Bot(){}

Bot::Bot(const Bot& other){(void)other;}

Bot& Bot::operator=(const Bot& other)
{
    if (this != &other)
        *this = other;
        
    return (*this);
}

Bot::~Bot(void)
{}

bool Bot::isCommand(const std::string& message) const
{
    return (message[0] == '!');
}

std::string Bot::executeCommand(const std::string& command, const Client& client, const Channel* channel) const
{
    if (!channel)
        _channel = false;
    else
        _channel = true;
    
    std::stringstream message;
    int index = getIndex(command);

    switch (index)
    {
        case 0:
            message << "Hello " << client.getNickname() << "!\n";
            break;

        case 1:
            message << "Current time is " << getTime() << ".\n";
            break;
            
        case 2:
            message << "Here are some useful information:\n" << getInfo();
            break;
            
        case 3:
            message << getJoke() << ".\n";
            break;
            
        case 4:
            message << displayChannels(channel);
            break;
            
        default:
            message << "That's not a command :P\n";
    }

    return (message.str());
}

bool Bot::getChannel(void) const
{
    return (this->_channel);
}