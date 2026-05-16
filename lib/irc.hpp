
#ifndef IRC_HPP
# define IRC_HPP

# include <map>
# include <vector>
# include <string>
# include <poll.h>
# include <cerrno>
# include <cstring>
# include <cstdlib>
# include <fcntl.h>
# include <netdb.h>
# include <sstream>
# include <signal.h>
# include <unistd.h>
# include <iostream>
# include <iterator>
# include "Server.hpp"
# include "Client.hpp"
# include <sys/stat.h>
# include "Channel.hpp"
# include "defines.hpp"
# include <sys/types.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <netinet/in.h>

std::vector<std::string> split(std::string str, std::string del);

#endif