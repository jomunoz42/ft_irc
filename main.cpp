
#include "irc.hpp"

int main(int ac, char **av) {
	if (ac == 1)
		return (std::cerr << "Missing port and password!" << std::endl, 1);
	if (ac == 2)
		return (std::cerr << "Missing password!" << std::endl, 1);
	if (ac > 3)
		return (std::cerr << "Too many arguments!" << std::endl, 1);
	int port = std::atoi(av[1]);
	if (port < MIN_PORT || port > MAX_PORT)
		return (std::cerr << ERR_PORT_RANGE << std::endl, 1);
	std::string server_name = av[0];
	server_name.erase(0, 2);
	Server irc(port, av[2], server_name);
	try {
		irc.start();
		irc.run();
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		irc.stop();
	}
	return (0);
}