
#include "irc.hpp"

void Server::start(void) {
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0) {
		std::string error = std::strerror(errno);
		throw std::runtime_error(std::string("socket() failed: ") + error);
	}
	this->setNonBlocking(this->_socket);
	int opt = 1;
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::string error = std::strerror(errno);
		throw std::runtime_error(std::string("setsockopt() failed: ") + error);
	}
	this->_ip_address.sin_family = AF_INET;
	this->_ip_address.sin_port = htons(this->_port);
	this->_ip_address.sin_addr.s_addr = INADDR_ANY;
	if (bind(this->_socket, reinterpret_cast<sockaddr*>(&this->_ip_address), sizeof(this->_ip_address)) < 0) {
		std::string error = std::strerror(errno);
		throw std::runtime_error(std::string("bind() failed: ") + error);
	}
	if (listen(this->_socket, 100) < 0) {
		std::string error = std::strerror(errno);
		throw std::runtime_error(std::string("listen() failed: ") + error);
	}
	this->addSocket(this->_socket);
	this->_server_running = true;
	std::cout << "IRC Server is setup!" << std::endl;
}

void Server::run(void) {
	std::cout << "IRC Server is running!" << std::endl;
	while (this->_server_running) {
		if (poll(this->_socket_list.data(), this->_socket_list.size(), -1) < 0) {
			std::string error = std::strerror(errno);
			throw std::runtime_error(std::string("poll() failed: ") + error);
		}
		for (size_t i = 0; i < this->_socket_list.size();) {
			if (this->_socket_list.at(i).revents & POLLIN) {
				int socket = this->_socket_list.at(i).fd;
				if (socket == this->_socket) {
					this->addClient();
					++i;
					continue ;
				}
				else {
					std::map<int, Client>::iterator j = this->_clients.find(socket);
					if (j != this->_clients.end()) {
						Client &client = j->second;
						size_t size_before = this->_socket_list.size();
						e_data ret = this->receiveClientData(client);
						if (ret == RECEIVED) {
							std::cout << "IRC Server has received data!" << std::endl;
							this->processData(client);
						}
						else if (ret == DISCONNECTED || ret == ERROR)
							this->removeClient(client);
						if (this->_socket_list.size() < size_before)
							continue ;
					}
				}
			}
			++i;
		}
	}
}

void Server::stop(void) {
	this->_server_running = false;
	close(this->_socket);
	for (std::map<int, Client>::iterator i = this->_clients.begin(); i != this->_clients.end(); ++i) {
		close(i->second.getSocket());
	}
}