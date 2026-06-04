/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbongiov <pbongiov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:23:54 by pbongiov          #+#    #+#             */
/*   Updated: 2026/06/04 19:36:33 by pbongiov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Channel.hpp"

void Bot::logIn(void)
{
    this->setNickname("Bob the Bot");
    this->setUsername("bot0");
    this->passwordAccepted();
}

Bot::Bot(int port, const std::string& password) : Client(-1), _port(port), _password(password)
{
    logIn();
}

Bot::Bot(const Bot& other){(void)other;}

Bot& Bot::operator=(const Bot& other)
{
    if (this != &other)
        *this = other;
        
    return (*this);
}

Bot::~Bot(void)
{}
