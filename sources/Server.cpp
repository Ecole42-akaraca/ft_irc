#include "../includes/Server.hpp"

Server::Server( int argc, char **argv )
	:	_isCheck(check( argc )),
		_port(port( argv[1] )),
		_password(password( argv[2] )),
		_host( "127.0.0.1" ),
		_isRun( true )
{
	std::cout << "Server Constructor called." << std::endl;

	Server::newSocket();
	std::cout << GREEN "Socket succesfully configured." END << std::endl;

}

Server::~Server( void )
{
	close(this->_serverFd); // Closing the server.
	std::cout << "Server succesfully closed!" << std::endl;
}

/* _________________________ MAIN FUCTION ___________________________________ */

/**
 * @brief 
 * 
 * @link: https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming
 * @return int 
 */
void		Server::newSocket( void )
{
	// <sys/socket.h>
	// AF_INET: Address Family IPv4. Internetwork: UDP, TCP, etc.
	// SOCK_STREAM: For TCP.
	// '0': Automatically choose the appropriate protocol.
	this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverFd < 0)
		throw std::runtime_error("Error while opening socket.");
	std::cout << YELLOW "Socket opened." END << std::endl;

	// SOL_SOCKET: Setting this SO_REUSEADDR flag to sockfd.
	// SO_REUSEADDR: Reuse local address.
	int val = 1;
	if (setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)))
		throw std::runtime_error("Error while setting socket options.");
	std::cout << YELLOW "Socket settings setted." END << std::endl;

	// fnctl: Manipulating the file description flag for the sockfd.
	// F_SETFL: /* set file status flags */
	// O_NONBLOCK: Make the socket non-blocking. (No delay).
	//if (fcntl(this->_socketfd, F_SETFL, O_NONBLOCK) == -1)
	//	throw std::runtime_error("Error while setting socket to NON-BLOCKING.");
	//std::cout << YELLOW "Socket set to NON-BLOCKING." END << std::endl;

	// <netinet/in.h>
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
	std::cout << YELLOW "Socket address settings setted." END << std::endl;

	// bind(): Assigns &serverAddress to sockfd address.
	if (bind(this->_serverFd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
		throw std::runtime_error("Error while binding socket.");
	std::cout << YELLOW "Socket address binded." END << std::endl;

	// listen(): First arg; Accepting incoming connection request;
	// listen(): Second arg; MAX_CONNECTIONS, maximum length of the
	// 	queue of pending connections.
	if (listen(this->_serverFd, MAX_CONNECTIONS) < 0)
		throw std::runtime_error("Error while listening on socket.");
	std::cout << YELLOW "Socket address setted listenning pending queue." END << std::endl;
	// return: Configured socketfd.
	std::cout << "_port: " << this->_port << " _password: " << this->_password\
		<< " _host: " << this->_host << " _serverFd: " << this->_serverFd << std::endl;
}

void	Server::start( void )
{
	pollfd serverFd;

	serverFd.fd = this->_serverFd;
	serverFd.events = POLLIN;
	serverFd.revents = 0;
	this->_pollfds.push_back(serverFd);

	// _pollfds.begin().base()->fd = this->_serverFd;
	// _pollfds.begin().base()->events = POLLIN;
	// _pollfds.begin().base()->revents = 0;
	std::cout << "Server listening port on: " << this->_port << std::endl;
	while (this->_isRun)
	{
		if (poll(this->_pollfds.begin().base(), this->_pollfds.size(), -1) < 0)
			throw std::runtime_error("Error while polling from fd.");
		for (itPoll it = this->_pollfds.begin(); it != this->_pollfds.end(); it++)
		{
			if (it->revents == 0) // 0 ise hicbir eylem gerceklestirilmeyecegini temsil eder.
				continue;

			if (it->revents & POLLHUP)
			{
				// onClientDisconnect(it->fd);
				break;
			}

			if (it->revents & POLLIN)
			{
				if (it->fd == this->_serverFd)
				{
					this->onClientConnect();
					break;
				}
				// onClientMessage(it->fd);
			}
		}
	}
}

void	Server::onClientConnect( void )
{
	int			clientFd;
	sockaddr_in	clientAddress;
	socklen_t	clientAddressSize = sizeof(clientAddress);

	clientFd = accept(this->_serverFd, (sockaddr *) &clientAddress, &clientAddressSize);
	if (clientFd < 0)
			throw std::runtime_error("Error while accepting on socket.");
	std::cout << YELLOW "Socket address setted incoming connections pending queue." END << std::endl;
	std::cout << "Accepted connection from " << inet_ntoa(clientAddress.sin_addr) << std::endl;

	pollfd clientPollFd;

	clientPollFd.fd = clientFd;
	clientPollFd.events = POLLIN;
	clientPollFd.revents = 0;
	this->_pollfds.push_back(clientPollFd);

	char	hostname[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *) &clientAddress, sizeof(clientAddress), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		throw std::runtime_error("Error while getting hostname on new client.");

	Client *client = new Client(clientFd, ntohs(clientAddress.sin_port), hostname);
	_clients.insert(std::make_pair(clientFd, client));

	// char message[1000];
	// snprintf(message, sizeof(message), "%s:%d has connected.", client->getHostname().c_str(), client->getPort());
}