#include "Bot.hpp"

/**
 * @brief Construct a new Bot:: Bot object
 * 
 * ./ircbot 127.0.0.1 8888 asdf
 * 
 * @param argc 
 * @param argv 
 */
Bot::Bot( int argc, char **argv )
	:
	_isCheck(check( argc )),
	_host( argv[1] ),
	_port(port( argv[2] )),
	_password(password( argv[3] )),
	_isRun( true ),
	_botNickname("ircBot"),
	_botUsername("Bot"),
	_botRealname("Poor Bot")
{
	std::cout << "Bot Constructor called." << std::endl;
	openSocket();
	connectSocketAddress();
	std::cout << GREEN "Bot Socket succesfully configured." END << std::endl;
}

Bot::~Bot( void )
{
	std::cout << "Bot succesfully closed!" << std::endl;
}

void	Bot::start( void )
{
	authenticate();
	listen();
	sleep(100);
	sendMessageToServer("QUIT");
	std::cout << "Bot created succesfully!" << std::endl;
}

void	Bot::authenticate( void )
{
	sendMessageToServer("CAP END");
	sendMessageToServer("PASS " + this->_password);
	sendMessageToServer("NICK " + _botNickname);
	// USER <username> <nickname> <hostname> :<realname>
	sendMessageToServer("USER " + _botUsername + " "\
		+ _botNickname + " " + _host + " :" + _botRealname);
}

void	Bot::sendMessageToServer( std::string message )
{
	std::cout << YELLOW << "BotResponse:>" + message << "<" << END << std::endl;
	message += "\r\n";
	if (send(this->_botFd, message.c_str(), message.size(), 0) == -1)
		throw (std::runtime_error( "Error: sendMessageToServer: Failed to send message." ));
}

void	Bot::listen( void )
{
	while (this->_isRun)
	{
		char buffer[MAX_BUFFER];
		ssize_t bytesRead = recv(this->_botFd, buffer, sizeof(buffer) - 1, 0);
		if (bytesRead > 0)
		{
			buffer[bytesRead] = '\0';
			std::cout << buffer << std::endl;
		}
	}
	close(this->_botFd);
}
