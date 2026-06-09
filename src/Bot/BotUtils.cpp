/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbongiov <pbongiov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 19:02:20 by pbongiov          #+#    #+#             */
/*   Updated: 2026/06/04 17:43:49 by pbongiov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Channel.hpp"
#include <ctime>
#include <fstream>
#include <cstdlib>

std::string getTime(void)
{
    std::time_t now = std::time(NULL);

    return(std::ctime(&now));
}

std::string getJoke(void)
{
    std::ifstream file("./src/Bot/jokes.txt");
    if (!file.is_open())
    {
        std::cerr << "Error opening jokes.txt" << std::endl;
        return (NULL);
    }
    
    std::srand(std::time(NULL));
    int n = std::rand() % 300;
    
    std::string line;
    int current_line = 0;

    while(std::getline(file, line))
    {
        if(current_line == n)
            return(line);
        ++current_line;
    }
    std::cerr << "Error: Not enough jokes in jokes.txt" << std::endl;
    return (NULL);
}

int getIndex(const std::string& s)
{
    if (s == "!hello")
        return (0);
    if (s == "!time")
        return (1);
    if (s == "!help")
        return (2);
    if (s == "!joke")
        return (3);
    if (s == "!channel")
        return (4);
    return (-1);
}

std::string getInfo(void)
{
    std::stringstream s;

    s << "\nPRIVMSG: Send a message to a User or Channel\n" << "NICK: Change User Nickname\n" << "USER: Set Username (<username> 0 * :<real_name>)\n" 
      << "JOIN: Join/ create a channel, !channel to see all channels\n\n";

    return (s.str());
}

// std::string displayChannels()
// {
    
// }

std::string displayChannels(void)
{
    return (std::string("List of channels: (to be implemented)\n"));
}
