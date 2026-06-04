/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotCommands.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbongiov <pbongiov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 19:36:14 by pbongiov          #+#    #+#             */
/*   Updated: 2026/06/04 19:36:27 by pbongiov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"


bool Bot::isCommand(const std::string& message) const
{
    return (message[0] == '!');
}

std::string Bot::executeCommand(const std::string& command, const Client& client) const
{
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
            message << displayChannels();
            break;
            
        default:
            message << "That's not a command :P\n";
    }

    return (message.str());
}