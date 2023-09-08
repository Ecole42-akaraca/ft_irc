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
	signal(SIGINT, sigHandler); // ^C signali icin.
	authenticate(); // Server'e katiliyoruz.
	std::cout << "Bot created succesfully!" << std::endl;
	checkChannels();
	sendMessageToServer("QUIT");
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

/**
 * @brief Bu fonksiyon server tarafindan gelen her mesajda calisacak.
 * 
 * @link https://docs.dal.net/docs/connection.html
 * 
 * @param arg 
 * @return void* 
 */
void*	Bot::listen( void* arg )
{
	Bot		*bot;
	ssize_t	bytesRead;
	char buffer[MAX_BUFFER];

	bot = static_cast<Bot *>(arg);
	while (bot->_isRun)
	{
		std::cout << "ben calisiyorm aq" << std::endl;
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
			exit(10050); // Network is down.
		}
		else
		{
			std::cerr << RED "Error: recv(): Socket error!" END << std::endl;
			close(bot->_botFd);
			pthread_exit((void*)-1);
			break;
		}
	}
	close(bot->_botFd);
	// return (NULL);
	pthread_exit(EXIT_SUCCESS); // 0
}

/**
 * @brief Burada Channel'lere bakacak. Eger baglanmadigi
 *  channel varsa ona baglanacak.
 * 
 * Her baglandiginda da Channel basina thread acacak.
 * Her thread bir channel'i tarayacak kontrol edecek.
 * 
 * 
 * pthread_create: Bu fonksiyon, yeni bir thread oluşturur.
 *  Fonksiyonun ilk parametresi, oluşturulan thread’in tanımlayıcısını (ID) tutacak bir değişkenin adresini alır.
 *  İkinci parametresi, thread’in özniteliklerini belirleyen bir yapıyı (attribute) alır.
 *  Üçüncü parametresi, thread’in çalıştıracağı fonksiyonu belirtir.
 *  Dördüncü parametresi ise, thread’in çalıştıracağı fonksiyona geçirilecek parametreleri belirtir.
 * 
 * pthread_join: Bu fonksiyon, belirtilen thread’in sonlanmasını bekler ve sonlandığında thread’in dönüş değerini alır.
 *  Fonksiyonun ilk parametresi, beklenen thread’in tanımlayıcısını (ID) alır.
 *  İkinci parametresi ise, thread’in dönüş değerini tutacak bir değişkenin adresini alır.
 * 
 * pthread_exit: Bu fonksiyon, çağrıldığı thread’i sonlandırır ve belirtilen dönüş değerini döndürür.
 *  Fonksiyonun tek parametresi, thread’in dönüş değerini belirtir.
 * 
 * pthread_detach: Bu fonksiyon, belirtilen thread’i ayrılmış (detached) olarak işaretler.
 *  Bir thread ayrıldığında, bu thread sonlandığında kaynakları otomatik
 *  olarak sistem tarafından geri alınır ve başka bir thread’in sonlanan
 *  thread ile birleşmesine (join) gerek kalmaz.
 * 
 * @fn joinChannels(): pthread'in fonksiyonu; void* dondurmeli ve aldigi
 *  parametre ( void* ) olmali.
 */
void	Bot::checkChannels( void )
{
	// Bakacak # ile baslayan channel varsa;
	//  o channel'e de baglanmadiysa baglanacak.

	pthread_t	botThreadID;
	std::string	getMessage;
	int			retResult; // *retResult

	if (pthread_create(&botThreadID, NULL, &listen, this) != 0) // Bir tane thread butun kendisine gelen mesajlari dinlerken.
		throw (std::runtime_error("pthread_create: Thread 'create' error!"));
	std::cout << __LINE__ << std::endl;

	// Bizde burada kanallara bakacagiz. Kanala girdigi anda zaten mesaj gelecegi icin listen o kanaldaki mesajlari da tarayip islemis olacak.
	while (true)
	{
		// sleep(2);
		if (getline(std::cin, getMessage))
			sendMessageToServer(getMessage);
	}

	// std::to_string(retResult).c_str() 
	if (pthread_join(botThreadID, (void **)&retResult) != 0) // Eger bu funciton'u acarsak yukaridaki functionun islemini bekler. Yani bu pthread'i acan thread islem yapmaz burada bekler.
		throw (std::runtime_error("pthread_join: Thread 'wait' error!"));
	if (pthread_detach(botThreadID) != 0) // Buradaki detach islemi olusturulan thread islemini bitirmeden gerceklesmemeli.
		throw (std::runtime_error("pthread_detach: Thread 'detach' error!"));
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