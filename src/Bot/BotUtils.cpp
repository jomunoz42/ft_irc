/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbongiov <pbongiov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 19:02:20 by pbongiov          #+#    #+#             */
/*   Updated: 2026/06/16 20:59:14 by pbongiov         ###   ########.fr       */
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
    std::string time = std::ctime(&now);

    if (!time.empty() && time[time.size() - 1] == '\n')
        time.erase(time.size() - 1);

    return time;
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
            return(line + "\r\n");
        ++current_line;
    }
    std::cerr << "Error: Not enough jokes in jokes.txt" << std::endl;
    return (NULL);
}

int getIndex(const std::string& s)
{
    if (s == "!hello\r\n")
        return (0);
    if (s == "!time\r\n")
        return (1);
    if (s == "!help\r\n")
        return (2);
    if (s == "!joke\r\n")
        return (3);
    if (s == "!list\r\n")
        return (4);
    return (-1);
}

std::string getInfo(void)
{
    std::stringstream s;

    s << "\nPRIVMSG: Send a message to a User/Bot or Channel\n" << "NICK: Change User Nickname\n" << "USER: Set Username (<username> 0 * :<real_name>)\n" 
      << "JOIN: Join/Create a channel, !channel to see all channels";

    return (s.str());
}

std::string listCommand(void)
{
    return (std::string("List of Bot commands:\n!hello\n!time\n!help\n!joke\n!list\n"));
}
