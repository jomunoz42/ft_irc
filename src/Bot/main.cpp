/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbongiov <pbongiov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:01:20 by pbongiov          #+#    #+#             */
/*   Updated: 2026/06/04 18:04:00 by pbongiov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Bot.hpp"

int main(int argc, char* argv[]) 
{
    if (argc != 4) 
    {
        std::cerr << "Usage: ./bot <port> <password>" << std::endl;
        return (1);
    }

    int port = std::atoi(argv[1]);
    std::string password = argv[2];

    Bot bot(port, password);
    return 0;
}

