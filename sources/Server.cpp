/**
 * @file Server.cpp
 * @author Görkem SEVER (gsever), Ahmet KARACA (akaraca)
 * @brief 
 * @version 0.1
 * @date 2023-08-27
 * 
 * @note: How messaging 2 client each other?
 * @link http://chi.cs.uchicago.edu/chirc/irc_examples.html
 * 
 * @note: All IRC commands.
 * @link http://www.ae.metu.edu.tr/~evren/history/documents/commands.htm
 * @link http://www.csun.edu/~webteach/mirc/commands.html
 * @link https://www.antionline.com/showthread.php?136933-IRC-flags
 * @link https://modern.ircdocs.horse/#irc-concepts
 * 
 * @copyright Copyright (c) 2023
 * 
 */
# include "../includes/Server.hpp"

Server::Server( int argc, char **argv )
	:	_isCheck(check( argc )),
		_port(port( argv[1] )),
		_password(password( argv[2] )),
		_host( "127.0.0.1" ),
		_isRun( true ),
		_serverName( "ft_irc")
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
	// _channels'ı silme
	for (itChannels it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second;
	_channels.clear();
	// _clients'ı silme
	for (itClients it = _clients.begin(); it != _clients.end(); ++it)
		delete it->second;
	_clients.clear(); // Map'i temizle
	for (size_t i = 0; i < _pollfds.size(); ++i) {
		close(_pollfds[i].fd); // Kapatılan dosyaları serbest bırakmak (isteğe bağlı)
	}
	_pollfds.clear();
	close(this->_serverFd); // Closing the server.
	std::cout << "Server succesfully closed!" << std::endl;
}

void	Server::start( void )
{
	Server::addToPollfds( this->_serverFd, POLLIN, 0 );
	std::cout << "Server listening on port: " << this->_port << std::endl;
	std::cout << "Server password: " << this->_password << std::endl;
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
				if (it->fd > 3 && it->fd < _SC_OPEN_MAX) // irssi client ile bağlandığım zaman, buraya gelen it->fd değeri BAYAAAAA bir fazla olduğundan kaynaklı, sınır belirleyip sadece nc localhost ile sinyal kestiğim client'ların bağlantısı için koydum.
				{
					Client *nc = _clients.at(it->fd);
					if (nc != NULL)
					{
						//if (nc->getIRCstatus() == CONNECTING)
							Server::quitReason(nc, "Signal FATAL ERROR!");
					}
				}
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

void	Server::acceptClients( void )
{
	sockaddr_in	clientAddress = {};
	socklen_t	clientAddressSize = sizeof(clientAddress);
	std::string	host;

	int	clientFd = accept(this->_serverFd, (sockaddr *) &clientAddress, &clientAddressSize);
	if (clientFd < 0)
		std::cerr << "Error while accepting client connection." << std::endl;
	else
	{
		Server::addToPollfds(clientFd, POLLIN, 0);
		host = inet_ntoa(clientAddress.sin_addr);
		struct hostent	*hostInfo = gethostbyname(host.c_str());
		Client	*client = new Client(clientFd, ntohs(clientAddress.sin_port), hostInfo->h_name);
		this->_clients.insert(std::make_pair(clientFd, client));
		std::cout << "New Client connected." << std::endl;
		std::cout << "Client's hostname: " << hostInfo->h_name << std::endl;
		std::cout << "Client's address type: " << hostInfo->h_addrtype << std::endl;
		std::cout << "Client's host length: " << hostInfo->h_length << std::endl;
		std::cout << "Client's port: " << ntohs(clientAddress.sin_port) << std::endl;
		std::cout << "Client's fd: " << clientFd << std::endl;
	}
}

/*
// std::vector<std::string> tokenArr = splitMessage(buffer);
// MODE gsever akaraca gorkem ahmet
*/
void	Server::commandHandler( itPoll &itClient )
{ 
	char buffer[MAX_BUFFER];
	std::vector<std::string>	tokenArr;
	ssize_t bytesRead = recv(itClient->fd, buffer, sizeof(buffer) - 1, 0);

	if (bytesRead > 0)
	{
		buffer[bytesRead] = '\0';
		std::map<std::string, std::string> tokens = splitMessage("\r\n", buffer);
		Client *at = _clients.at(itClient->fd);
		for(itSplit itToken = tokens.begin(); itToken != tokens.end(); ++itToken)
		{
			std::cout << BLUE << "Message:>" << itToken->first << "-" << itToken->second << "<" << END << std::endl;
			for (itCmdFunc itCF = t_cmdFunc.begin(); itCF != t_cmdFunc.end(); ++itCF)
			{
				if (itToken->first.compare(itCF->first) == 0)
				{
					std::vector<std::string> cmd = cmdMessage(itToken->second);
					cmd.insert(cmd.begin(), itToken->first);
					(this->*(itCF->second))(at, cmd);
					cmd.clear();
					break;
				}
			}
		}
		tokens.clear();
	}
}

void	Server::removeClient(int clientFd)
{
	itClients it = _clients.find(clientFd);
	if (it != _clients.end())
	{
		delete it->second; // Belleği serbest bırak
		_clients.erase(it);
	}
}

void	Server::removeChannel( std::string channelName )
{
	itChannels it = this->_channels.find(channelName);
	if (it != _channels.end())
	{
		std::cout << it->second->getName() << ": Channel removed." << std::endl;
		_channels.erase(it);
	}
}

void	Server::quitReason( Client* client, std::string message )
{
	std::vector<std::string> msg;
	msg.push_back(message);
	Server::quit(client, msg);
}

void	Server::serverInfo( void )
{
	std::cout << BLUE << "------------POLL.FDS-----------------" << END << std::endl;
	for (size_t i = 0; i < this->_pollfds.size(); i++)
		std::cout << this->_pollfds[i].fd << " - ";
	std::cout << BLUE << "\n------------CLIENTS.FDS--------------" << END << std::endl;
	for (itClients it = _clients.begin(); it != _clients.end(); it++)
		std::cout << it->first << " - ";
	std::cout << BLUE << "\n------------CHANNELS-----------------" << END << std::endl;
	for (itChannels it = _channels.begin(); it != _channels.end(); it++)
		std::cout << it->first << " - ";
	std::cout << BLUE << "\n-------------------------------------" << END << std::endl;
}
