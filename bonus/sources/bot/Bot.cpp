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
	signal(SIGINT, sigHandler);
	authenticate();
	checkChannels();
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

void*	Bot::listen( void* arg )
{
	Bot		*bot;
	ssize_t	bytesRead;
	char buffer[MAX_BUFFER];

	bot = static_cast<Bot *>(arg);
	while (bot->_isRun)
	{
		bytesRead = recv(bot->_botFd, buffer, sizeof(buffer) - 1, 0);
		if (bytesRead > 0)
		{
			buffer[bytesRead] = '\0';
			std::cout << "-->" << buffer << std::endl;
			// std::cout.write(buffer, bytesRead);
		}
		else if (bytesRead == 0)
		{
			std::cerr << RED "Connection lost!" END << std::endl;
			exit(0);
		}
		else
		{
			std::cerr << RED "Error: recv(): Socket error!" END << std::endl;
			break;
		}
	}
	close(bot->_botFd);
	return (NULL);
}

/**
 * @brief Burada Channel'lere bakacak. Eger baglanmadigi
 *  channel varsa ona baglanacak.
 * 
 * Her baglandiginda da Channel basina thread acacak.
 * Her thread bir channel'i tarayacak kontrol edecek.
 * 
 * @fn joinChannels(): pthread'in fonksiyonu; void* dondurmeli ve aldigi
 *  parametre ( void* ) olmali.
 */
void	Bot::checkChannels( void )
{
	// Bakacak # ile baslayan channel varsa;
	//  o channel'e de baglanmadiysa,
	//  thread acacak be baglanacak.
	// for (itChannels itC = this->_channels.begin();
		// itC != this->_channels.end(); itC++)
	// {
		pthread_t	botThreadID;

		pthread_create(&botThreadID, NULL, &listen, this);
		pthread_join(botThreadID, NULL);
		pthread_detach(botThreadID);
		std::cout << __LINE__ << std::endl;
	// }
	

}

/**
 * @brief Biz olusturdugumuz 'thread'a bu function
 *  uzerinde calismasini sagliyoruz.
 * 
 * Bunun saglanabilmesi icin;
 * 
 * @param arg 
 * @return void* 
 */
void	Bot::joinChannels( void )
{

}

void	Bot::sigHandler( int signalNum )
{
	if (signalNum == SIGINT)
	{
		std::cout << "Interrupt signal found! Bot terminating..." << std::endl;
		exit(signalNum);
	}
}