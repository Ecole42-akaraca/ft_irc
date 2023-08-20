/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaraca <akaraca@student.42.tr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:29:23 by gsever            #+#    #+#             */
/*   Updated: 2023/08/20 13:46:51 by akaraca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server( int argc, char **argv )
	: _port(static_cast<unsigned short>(std::atoi(argv[1]))),
	_password(argv[2]),
	_host("127.0.0.1")
{
	(void)argc;
	std::cout << "Server Constructor called." << std::endl;
	this->_socket = Server::newSocket();
	std::cout << GREEN "Socket succesfully configured." END << std::endl;
}

Server::~Server( void ) {}

/* _________________________ MAIN FUCTION ___________________________________ */
void	Server::start( void )
{
	std::cout << "_port: " << this->_port << " _password: " << this->_password\
		<< " _host: " << this->_host << " _socketfd: " << this->_socket << std::endl;

}

/**
 * @brief 
 * 
 * @link: https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming
 * @return int 
 */
int		Server::newSocket( void )
{
	// <sys/socket.h>
	// AF_INET: Address Family IPv4. Internetwork: UDP, TCP, etc.
	// SOCK_STREAM: For TCP.
	// '0': Automatically choose the appropriate protocol.
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw std::runtime_error("Error while opening socket.");
	std::cout << YELLOW "Socket opened." END << std::endl;

	// SOL_SOCKET: Setting this SO_REUSEADDR flag to sockfd.
	// SO_REUSEADDR: Reuse local address.
	int val = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)))
		throw std::runtime_error("Error while setting socket options.");
	std::cout << YELLOW "Socket settings setted." END << std::endl;

	// fnctl: Manipulating the file description flag for the sockfd.
	// F_SETFL: /* set file status flags */
	// O_NONBLOCK: Make the socket non-blocking. (No delay).
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error while setting socket to NON-BLOCKING.");
	std::cout << YELLOW "Socket set to NON-BLOCKING." END << std::endl;

	// <netinet/in.h>
	struct sockaddr_in socketAddress = {};

	// Setting 0 &socketAddress's sizeof() bytes.
	memset((char *) &socketAddress, 0, sizeof(socketAddress));
	std::cout << YELLOW "Socket address created." END << std::endl;

	// INADDR_ANY: Bind all interfaces.
	// If you wish to bind your socket to localhost only, the syntax would be my_sockaddress.sin_addr.s_addr = inet_addr("127.0.0.1");,
	// htons(): Converts the unsigned short integer this->_port from host byte order to network byte order.
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = INADDR_ANY;
	socketAddress.sin_port = htons(this->_port);
	std::cout << YELLOW "Socket address settings setted." END << std::endl;

	// bind(): Assigns &socketAddress to sockfd address.
	if (bind(sockfd, (struct sockaddr *) &socketAddress, sizeof(socketAddress)) < 0)
		throw std::runtime_error("Error while binding socket.");
	std::cout << YELLOW "Socket address binded." END << std::endl;

	// listen(): First arg; Accepting incoming connection request;
	// listen(): Second arg; MAX_CONNECTIONS, maximum length of the
	// 	queue of pending connections.
	if (listen(sockfd, MAX_CONNECTIONS) < 0)
		throw std::runtime_error("Error while listening on socket.");
	std::cout << YELLOW "Socket address setted listenning pending queue." END << std::endl;
	// return: Configured socketfd.
	return (sockfd);
}
/* -------------------------------------------------------------------------- */
/* _________________________ SET/GET FUNCTIONS ______________________________ */
/* -------------------------------------------------------------------------- */
