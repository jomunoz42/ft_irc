/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbongiov <pbongiov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:01:20 by pbongiov          #+#    #+#             */
/*   Updated: 2026/06/16 17:46:05 by pbongiov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main(int ac, char** av)
{
    Bot dummyBot(0, "");

    switch (ac)
    {
        case 3:
            dummyBot.setHost("127.0.0.1");
            dummyBot.setPort(std::atoi(av[1]));
            dummyBot.setPassword(av[2]);
            break;
        case 4:
            dummyBot.setHost(av[1]);
            dummyBot.setPort(std::atoi(av[2]));
            dummyBot.setPassword(av[3]);
            break;
        default:
            std::cerr << "Usage: ./bot <port> <password>\n";
            std::cerr << "       ./bot <host> <port> <password>\n";
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
