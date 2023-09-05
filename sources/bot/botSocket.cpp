#include "Bot.hpp"

void	Bot::openSocket( void )
{
	// <sys/socket.h>
	// AF_INET: Address Family IPv4. Internetwork: UDP, TCP, etc.
	// SOCK_STREAM: For TCP.
	// '0': Automatically choose the appropriate protocol.
	this->_botFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_botFd < 0)
		throw std::runtime_error("Error while opening socket.");
	std::cout << YELLOW "Socket opened." END << std::endl;
}

void	Bot::connectSocketAddress( void )
{
	struct hostent* server_info = gethostbyname(this->_host.c_str());
	if (!server_info)
	{
		throw std::runtime_error("Host lookup failed.");
		close(this->_botFd);
		return ;
	}

	struct sockaddr_in botAddress;

	// Setting 0 &botAddress's sizeof() bytes.
	memset((char *) &botAddress, 0, sizeof(botAddress));
	std::cout << YELLOW "Socket address created." END << std::endl;

	botAddress.sin_family = AF_INET;
	botAddress.sin_addr.s_addr = inet_addr(_host.c_str());
	botAddress.sin_port = htons(this->_port);
	memcpy(&botAddress.sin_addr.s_addr, server_info->h_addr, server_info->h_length);
	std::cout << YELLOW "Socket address settings set." END << std::endl;

	if (connect(this->_botFd, (struct sockaddr *) &botAddress, sizeof(botAddress)) < 0)
		throw std::runtime_error("Error while connecting to host.");
	std::cout << YELLOW << "Connected to " << this->_host << " on port " << this->_port << END <<  std::endl;
}
