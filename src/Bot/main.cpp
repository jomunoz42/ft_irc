/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbongiov <pbongiov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:01:20 by pbongiov          #+#    #+#             */
/*   Updated: 2026/06/09 20:00:00 by GitHub Copilot   ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main(int ac, char** av)
{
    Bot dummyBot(0, "");

    if (ac == 3)
    {
        dummyBot.setHost("127.0.0.1");
        dummyBot.setPort(std::atoi(av[1]));
        dummyBot.setPassword(av[2]);
    }
    else if (ac == 4)
    {
        dummyBot.setHost(av[1]);
        dummyBot.setPort(std::atoi(av[2]));
        dummyBot.setPassword(av[3]);
    }
    else
    {
        std::cerr << "Usage: ./bot <port> <password>\n";
        std::cerr << "       ./bot <host> <port> <password>\n";
        return 1;
    }

    if (dummyBot.getPort() <= 0)
    {
        std::cerr << "Invalid port." << std::endl;
        return 1;
    }

    int socket_fd = dummyBot.connectToServer();
    if (socket_fd < 0)
        return 1;

    std::cout << "Connected to " << dummyBot.getHost() << ":" << dummyBot.getPort() << std::endl;

    std::cout << "Step 1: send PASS command" << std::endl;
    if (!dummyBot.sendLine(socket_fd, "PASS " + dummyBot.getPassword()))
        return 1;
    dummyBot.receiveResponse(socket_fd);

    std::cout << "Step 2: send NICK command" << std::endl;
    if (!dummyBot.sendLine(socket_fd, "NICK Bot"))
        return 1;
    dummyBot.receiveResponse(socket_fd);

    std::cout << "Step 3: send USER command" << std::endl;
    if (!dummyBot.sendLine(socket_fd, "USER bot0 0 * :Bot"))
        return 1;
    dummyBot.receiveResponse(socket_fd);

    close(socket_fd);
    return 0;
}
