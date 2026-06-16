/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbongiov <pbongiov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:23:54 by pbongiov          #+#    #+#             */
/*   Updated: 2026/06/16 20:43:40 by pbongiov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Channel.hpp"

Bot::Bot(int port, const std::string& password) : Client(0), _port(port), _password(password){}

Bot::Bot(const Bot& other) : Client(other), _port(other._port), _password(other._password), _host(other._host){}

Bot& Bot::operator=(const Bot& other)
{
    if (this != &other)
    {
        Client::operator=(other);
        _port = other._port;
        _password = other._password;
        _host = other._host;
    }
    return (*this);
}

Bot::~Bot(void){}

void Bot::setPort(int port){ _port = port; }

void Bot::setPassword(const std::string& password){ _password = password; }

void Bot::setHost(const std::string& host){ _host = host; }

void Bot::setName(const std::string& name){ _name = name; }

std::string Bot::getHost(void) const { return _host; }

std::string Bot::getPassword(void) const { return _password; }

int Bot::getPort(void) const { return _port; }
