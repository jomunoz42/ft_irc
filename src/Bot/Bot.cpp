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

Bot::Bot(void) : Client(-1), _port(0), _password("")
{
}

Bot::Bot(int port, const std::string& password) : Client(-1), _port(port), _password(password)
{
}

Bot::Bot(const Bot& other) : Client(other), _port(other._port), _password(other._password), _host(other._host)
{
}

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

Bot::~Bot(void)
{}

void Bot::setPort(int port)
{
    _port = port;
}

void Bot::setPassword(const std::string& password)
{
    _password = password;
}

void Bot::setHost(const std::string& host)
{
    _host = host;
}

std::string Bot::getHost(void) const
{
    return _host;
}

std::string Bot::getPassword(void) const
{
    return _password;
}

int Bot::getPort(void) const
{
    return _port;
}

bool Bot::sendAll(int socket_fd, const std::string& data)
{
    const char* bytes = data.c_str();
    size_t total = 0;
    size_t length = data.size();

    while (total < length)
    {
        ssize_t sent = send(socket_fd, bytes + total, length - total, 0);
        if (sent < 0)
        {
            if (errno == EINTR)
                continue;
            std::cerr << "send() failed: " << std::strerror(errno) << std::endl;
            return false;
        }
        total += static_cast<size_t>(sent);
    }
    return true;
}

bool Bot::sendLine(int socket_fd, const std::string& line)
{
    std::string message = line;
    if (message.find("\r\n") == std::string::npos)
        message += "\r\n";
    return sendAll(socket_fd, message);
}

int Bot::connectToServer(void)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        std::cerr << "socket() failed: " << std::strerror(errno) << std::endl;
        return -1;
    }

    sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(_port);

    if (inet_pton(AF_INET, _host.c_str(), &server_address.sin_addr) <= 0)
    {
        hostent* host_info = gethostbyname(_host.c_str());
        if (host_info == NULL)
        {
            std::cerr << "Failed to resolve host: " << _host << std::endl;
            close(socket_fd);
            return -1;
        }
        std::memcpy(&server_address.sin_addr, host_info->h_addr, host_info->h_length);
    }

    if (connect(socket_fd, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) < 0)
    {
        std::cerr << "connect() failed: " << std::strerror(errno) << std::endl;
        close(socket_fd);
        return -1;
    }

    return socket_fd;
}

void Bot::receiveResponse(int socket_fd)
{
    char buffer[1024];
    ssize_t received = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (received < 0)
    {
        std::cerr << "recv() failed: " << std::strerror(errno) << std::endl;
        return;
    }
    if (received == 0)
    {
        std::cerr << "Server closed connection." << std::endl;
        return;
    }
    buffer[received] = '\0';
    std::cout << buffer;
}