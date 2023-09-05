#include "../includes/Server.hpp"

void	Server::openSocket( void )
{
	// <sys/socket.h>
	// AF_INET: Address Family IPv4. Internetwork: UDP, TCP, etc.
	// SOCK_STREAM: For TCP.
	// '0': Automatically choose the appropriate protocol.
	this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverFd < 0)
		throw std::runtime_error("Error while opening socket.");
	std::cout << YELLOW "Socket opened." END << std::endl;
}

void	Server::setSocketOptions( void )
{
	// SOL_SOCKET: Setting this SO_REUSEADDR flag to sockfd.
	// SO_REUSEADDR: Reuse local address.
	int val = 1;
	if (setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)))
		throw std::runtime_error("Error while setting socket options.");
	std::cout << YELLOW "Socket settings set." END << std::endl;
}

void	Server::createSocketAddress( void )
{
	struct sockaddr_in serverAddress;

	// Setting 0 &serverAddress's sizeof() bytes.
	memset((char *) &serverAddress, 0, sizeof(serverAddress));
	std::cout << YELLOW "Socket address created." END << std::endl;

	// INADDR_ANY: Bind all interfaces.
	// If you wish to bind your socket to localhost only, the syntax would be my_sockaddress.sin_addr.s_addr = inet_addr("127.0.0.1");,
	// htons(): Converts the unsigned short integer this->_port from host byte order to network byte order.
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(this->_port);
	std::cout << YELLOW "Socket address settings set." END << std::endl;

	// bind(): Assigns &serverAddress to sockfd address.
	if (bind(this->_serverFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
		throw std::runtime_error("Error while binding socket.");
	std::cout << YELLOW "Socket address bound." END << std::endl;
}

void	Server::startListening( void )
{
	// listen(): First arg; Accepting incoming connection request;
	// listen(): Second arg; MAX_CONNECTIONS, maximum length of the
	// 	queue of pending connections.
	if (listen(this->_serverFd, MAX_CONNECTIONS) < 0)
		throw std::runtime_error("Error while listening on socket.");
	std::cout << YELLOW "Socket listening with a pending queue." END << std::endl;
}
