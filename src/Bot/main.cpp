/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbongiov <pbongiov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:01:20 by pbongiov          #+#    #+#             */
/*   Updated: 2026/06/16 21:15:15 by pbongiov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main(int ac, char** av)
{
    Bot dummyBot(0, "");
    
    switch (ac)
    {
        case 4:
            dummyBot.setHost("127.0.0.1");
            dummyBot.setPort(std::atoi(av[1]));
            dummyBot.setPassword(av[2]);
            dummyBot.setName(av[3]);
            break;
            
        case 5:
            dummyBot.setHost(av[1]);
            dummyBot.setPort(std::atoi(av[2]));
            dummyBot.setPassword(av[3]);
            dummyBot.setName(av[4]);
            break;
            
        default:
            std::cerr << "Usage: ./bot <port> <password> <bot_name>\n";
            std::cerr << "       ./bot <host> <port> <password> <bot_name>\n";
            return 1;
    }
    
    if (dummyBot.getPort() <= 0)
    {
        std::cerr << "Invalid port." << std::endl;
        return 1;
    }

    dummyBot.run();
    return 0;
}
