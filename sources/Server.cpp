#include "../includes/Server.hpp"

Server::Server( int argc, char **argv )
	:	_isCheck(check( argc )),
		_port(port( argv[1] )),
		_password(password( argv[2] )),
		_host( "127.0.0.1" ),
		_isRun( true )
{
	std::cout << "Server Constructor called." << std::endl;
	openSocket();
	setSocketOptions();
	createSocketAddress();
	startListening();
	initCommands();
	// std::cout << welcomeServer() << std::endl;
	std::cout << GREEN "Socket succesfully configured." END << std::endl;
}

Server::~Server( void )
{
	t_cmdFunc.clear();
	for (size_t i = 0; i < _pollfds.size(); ++i) {
		close(_pollfds[i].fd); // Kapatılan dosyaları serbest bırakmak (isteğe bağlı)
	}
	_pollfds.clear();

	// _clients'ı silme
	for (itClients it = _clients.begin(); it != _clients.end(); ++it)
		delete it->second;
	_clients.clear(); // Map'i temizle
	// _channels'ı silme
	for (itChannels it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second;
	_channels.clear();
	close(this->_serverFd); // Closing the server.
	std::cout << "Server succesfully closed!" << std::endl;
	exit(0);
}

/* _________________________ MAIN FUCTION ___________________________________ */

void	Server::start( void )
{
	Server::addToPollfds( this->_serverFd, POLLIN, 0 );
	std::cout << "Server listening on port: " << this->_port << std::endl;
	while (this->_isRun)
	{
		if (poll(_pollfds.begin().base(), _pollfds.size(), -1) < 0)
			throw std::runtime_error("Error while polling from fd.");
		for (itPoll it = _pollfds.begin(); it != _pollfds.end(); it++)
		{
			if (it->revents == 0)
				continue;

			if (it->revents & POLLHUP)
			{
				std::cout << "Client disconnected." << std::endl;
				break; // Move to the next socket
			}

			if (it->revents & POLLIN)
			{
				if (it->fd == _serverFd)
				{ 
					Server::acceptClients();
					break;
				}
				Server::commandHandler(it);
			}
		}
	}
}

// std::vector<std::string> tokenArr = splitMessage(buffer);
// MODE gsever akaraca gorkem ahmet
void	Server::commandHandler( itPoll &itClient )
{
	char buffer[1024];
	std::vector<std::string>	tokenArr;
	ssize_t bytesRead = recv(itClient->fd, buffer, sizeof(buffer) - 1, 0);

	if (bytesRead > 0)
	{
		buffer[bytesRead] = '\0';
		std::map<std::string, std::string> tokens = splitMessage(buffer);
		Client *at = _clients.at(itClient->fd);
		for(std::map<std::string, std::string>::iterator itToken = tokens.begin(); itToken != tokens.end(); ++itToken)
		{
			std::cout << BLUE << "Message:>" << itToken->first << "-" << itToken->second << "<" << END << std::endl;
			std::map<std::string, CommandFunction>::iterator itFunc;
			for (itFunc = t_cmdFunc.begin(); itFunc != t_cmdFunc.end(); ++itFunc) {
				if (itToken->first.compare(itFunc->first) == 0)
				{					
					std::vector<std::string> cmd = cmdMessage(itToken->second);
					cmd.insert(cmd.begin(), itToken->first);

	// std::cout << B_BLUE << "Tokens:>";
	// for (size_t i = 0; i < cmd.size(); i++)
	// 	std::cout << "`" << cmd[i] << "`";
	// std::cout << "<" << END << std::endl;

					// std::cout << "......." << at->getFd() << std::endl;
					(this->*(itFunc->second))( at, cmd);
					cmd.clear();
					break;
				}
			}
		}
		tokens.clear();
	}
}

/**
 * @brief 
 * 
 * struct hostent;
 * 
 */
void	Server::acceptClients( void )
{
	sockaddr_in	clientAddress = {};
	socklen_t	clientAddressSize = sizeof(clientAddress);
	std::string	host;

	int	clientFd = accept(this->_serverFd, (sockaddr *) &clientAddress, &clientAddressSize);
	if (clientFd < 0)
	{
		std::cerr << "Error while accepting client connection." << std::endl;
	}
	else
	{
		Server::addToPollfds(clientFd, POLLIN, 0);
		host = inet_ntoa(clientAddress.sin_addr);
		struct hostent	*hostInfo = gethostbyname(host.c_str());
		Client	*client= new Client(clientFd, ntohs(clientAddress.sin_port), hostInfo->h_name);
		this->_clients.insert(std::make_pair(clientFd, client));
		std::cout << "New Client connected." << std::endl;
		std::cout << "Client's hostname: " << hostInfo->h_name << std::endl;
		std::cout << "Client's address type: " << hostInfo->h_addrtype << std::endl;
		std::cout << "Client's host length: " << hostInfo->h_length << std::endl;
		std::cout << "Client's port: " << ntohs(clientAddress.sin_port) << std::endl;
	}
}

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
