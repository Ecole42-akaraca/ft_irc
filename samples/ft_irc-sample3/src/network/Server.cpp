#include "network/Server.hpp"

Server::Server(const std::string &port, const std::string &password)
		: _running(1), _host("127.0.0.1"), _port(port), _password(password) {

	_sock = newSocket();
	_commandHandler = new CommandHandler(this);
}

Server::~Server() {
	delete _commandHandler;
}

void Server::start() {
	pollfd server_fd = {_sock, POLLIN, 0};
	_pollfds.push_back(server_fd);

	ft_log("Server listening...");

	while (_running) {

		if (poll(_pollfds.begin().base(), _pollfds.size(), -1) < 0)
			throw std::runtime_error("Error while polling from fd.");

		for (pollfds_iterator it = _pollfds.begin(); it != _pollfds.end(); it++) {

			if (it->revents == 0)
				continue;

			if ((it->revents & POLLHUP) == POLLHUP) {
				onClientDisconnect(it->fd);
				break;
			}

			if ((it->revents & POLLIN) == POLLIN) {
				if (it->fd == _sock) {
					onClientConnect();
					break;
				}
				onClientMessage(it->fd);
			}
		}
	}
}

int Server::newSocket() {

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "Socket olusturduk morq!!!!!!!!!!!!!!!!!!!!!!!" << std::cout << std::endl;
	if (sockfd < 0)
		throw std::runtime_error("Error while opening socket.");

	int val = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)))
		throw std::runtime_error("Error while setting socket options.");

	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
		throw std::runtime_error("Error while setting socket to NON-BLOCKING.");
	}

	struct sockaddr_in serv_address = {};

	memset((char *) &serv_address, 0, sizeof(serv_address));

	serv_address.sin_family = AF_INET;
	serv_address.sin_addr.s_addr = INADDR_ANY;
	serv_address.sin_port = htons(std::atoi(_port.c_str()));

	if (bind(sockfd, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0)
		throw std::runtime_error("Error while binding socket.");

	if (listen(sockfd, MAX_CONNECTIONS) < 0)
		throw std::runtime_error("Error while listening on socket.");
	return sockfd;
}

std::string Server::readMessage(int fd) {

	std::string message;

	char buffer[100];
	memset(buffer, 0, 100);

	while (!std::strstr(buffer, "\r\n")) {
		memset(buffer, 0, 100);

		if (recv(fd, buffer, 100, 0) < 0) {
			if (errno != EWOULDBLOCK)
				throw std::runtime_error("Error while reading buffer from client.");
		}

		message.append(buffer);
	}

	return message;
}

void Server::onClientConnect() {
	int fd;
	sockaddr_in s_address = {};
	socklen_t s_size = sizeof(s_address);

	fd = accept(_sock, (sockaddr *) &s_address, &s_size);
	if (fd < 0)
		throw std::runtime_error("Error while accepting new client.");

	pollfd pollfd = {fd, POLLIN, 0};
	_pollfds.push_back(pollfd);

	char hostname[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *) &s_address, sizeof(s_address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		throw std::runtime_error("Error while getting hostname on new client.");

	Client *client = new Client(fd, hostname, ntohs(s_address.sin_port));
	_clients.insert(std::make_pair(fd, client));

	char message[1000];
	std::cout << "BIRI BAGLANDI ABI" << std::endl << std::flush;
	// sprintf(message, "%s:%d has connected.", client->getHostname().c_str(), client->getPort());
	snprintf(message, sizeof(message), "%s:%d has connected.", client->getHostname().c_str(), client->getPort());
	ft_log(message);
}

void Server::onClientDisconnect(int fd) {

	try {

		Client *client = _clients.at(fd);
		client->leave();

		char message[1000];
		// sprintf(message, "%s:%d has disconnected.", client->getHostname().c_str(), client->getPort());
		snprintf(message, sizeof(message), "%s:%d has disconnected.", client->getHostname().c_str(), client->getPort());
		ft_log(message);

		_clients.erase(fd);

		for (pollfds_iterator it = _pollfds.begin(); it != _pollfds.end(); it++) {
			if (it->fd != fd)
				continue;
			_pollfds.erase(it);
			close(fd);
			break;
		}

		delete client;
		std::cout << "ne oldu ya baglantin gitti abi" << std::endl << std::flush;
	}
	catch (const std::out_of_range &ex) {
	}
}

void Server::onClientMessage(int fd) {
	try {
		std::cout << "Mesaj atildii" << std::endl << std::flush;
		Client *client = _clients.at(fd);
		_commandHandler->invoke(client, readMessage(fd));
	}
	catch (const std::out_of_range &ex) {
	}
}

Client *Server::getClient(const std::string &nickname) {
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (!nickname.compare(it->second->getNickname()))
			return it->second;
	}
	return NULL;
}

Channel *Server::getChannel(const std::string &name) {

	for (channels_iterator it = _channels.begin(); it != _channels.end(); it++)
		if (it.operator*()->getName() == name)
			return it.operator*();

	return NULL;
}

Channel *Server::createChannel(const std::string &name, const std::string &password, Client *client) {

	Channel *channel = new Channel(name, password, client);
	_channels.push_back(channel);

	return channel;
}
