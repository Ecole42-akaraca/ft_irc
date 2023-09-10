#include "Bot.hpp"

std::map<std::string, std::string> Bot::Bot::_dataBadWords;

int Bot::_botFd = -1;

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

/**
 * @brief Gelen girdi .tolower() seklinde geldikten sonra
 *  burada stupid ise ****** olarak return edebiliri.
 * 
 */
void	Bot::initBadWords( void )
{
	Bot::_dataBadWords["stupid"] = "******";
	Bot::_dataBadWords["idiot"] = "*****";
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
	if (send(Bot::getFd(), message.c_str(), message.size(), 0) == -1)
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

	initBadWords();
	bot = static_cast<Bot *>(arg);
	while (bot->_isRun)
	{
		std::cout << "Listen'e veri geldi yani mesaj." << std::endl;
		bytesRead = recv(bot->_botFd, buffer, sizeof(buffer) - 1, 0);
		if (bytesRead > 0)
		{
			buffer[bytesRead] = '\0';
			std::cout << "-->" << buffer << std::endl;
			// std::cout.write(buffer, bytesRead);
			Bot::onMessageReceive(buffer);
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

/**
 * @brief 
 * 
 * @note BotTokens:>`:gsever!~gsever@127.0.0.1``PRIVMSG``#asdf``:selam`<
 * 	for[0]: :gsever!~gsever@127.0.0.1	-> Mesaji gonderen kisi.
 * 	for[1]: PRIVMSG						-> Command
 * 	for[2]: #asdf						-> Channel
 * 	for[3]: :selam						-> Command arg
 * 
 * 1- [0] Mesaji gonderen kisinin token'ninde kotu soz
 *  var ise Channel'den kicklenecek.
 * 2- [3] Mesajin icerigininin toknen'inde kotu soz
 *  var ise Channel'den kicklenecek.
 * 
 * @param buffer 
 */
void	Bot::onMessageReceive( std::string buffer )
{
	std::vector<std::string>	tokens;
	std::string					nickname;

	tokens = Bot::tokenMessage(buffer); // Gelen mesajin tamamini tokenlerine ayiriyoruz.
	nickname = Bot::scanNickname(tokens[0]); // Ilk tokenimizin icerisindeki :gsever!~gsever@127.0.0.1 icerisinden : ile ! arasindaki nickname'yi aliyoruz.
	if (nickname == "")
		return ;
	if (tokens[3][0] == ':')
		tokens[3].erase(0, 1); // En basindaki ':'i siliyor.
	for (size_t i = 0; i < tokens.size(); i++)
	{
		// Burada token[3]'e mesajin baslangici : ile basladigi icin badword'le karsilasitirirken eslesmiyor.
		if (Bot::_dataBadWords.find(Bot::toLowerCase(tokens[i])) != Bot::_dataBadWords.end())
		{
			std::cout << "Bad Words found: " << tokens[i] << std::endl;
			Bot::sendMessageToServer("KICK " + tokens[2] + " " + nickname);
		}
	}
}

/**
 * @brief Disaridan aldigi kelimenin butun karakterlerini
 *  kucuk harfe ceviriyor.
 * 
 * @param word 
 * @return std::string 
 */
std::string Bot::toLowerCase( const std::string& string )
{
	std::string result;
	result = string;

	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

// std::string	Bot::toLowerCase( std::string word )
// {
// 	std::string	loweredString;

// 	loweredString = "";
// 	std::cout << "word;" << word << std::endl;
// 	for (size_t i = 0; i < word.size(); i++)
// 		loweredString[i] = std::tolower(word[i]);
// 	std::cout << "LoweredStriing:" << loweredString << std::endl;
// 	return (loweredString);
// }

// // Function to replace bad words in a string
// std::string	Bot::filterBadWords( std::string& text )
// {
// 	// std::string result = text;
// 	// for (std::map<std::string, std::string>::iterator it = Bot::_dataBadWords.begin(); it != Bot::_dataBadWords.end(); ++it) {
// 	// 	std::string word = it->first;
// 	// 	std::string lowerCaseWord = toLowerCase(word);
// 	// 	// Find the bad word in the text
// 	// 	size_t pos = toLowerCase(result).find(lowerCaseWord);
// 	// 	while (pos != std::string::npos) {
// 	// 		// Replace the bad word with asterisks
// 	// 		result.replace(pos, word.length(), it->second);
// 	// 		// Find the next occurrence of the bad word
// 	// 		pos = toLowerCase(result).find(lowerCaseWord, pos + 1);
// 	// 	}
// 	// }
// 	// return result;


// }

std::string	Bot::scanNickname( std::string message )
{
	size_t		start;
	size_t		end;
	std::string	result;

	start = message.find(":");
	end = message.find("!");
	result = "";
	if (start != std::string::npos && end != std::string::npos)
		result = message.substr(start + 1, end - start - 1);
	return (result);
}

std::vector<std::string>	Bot::tokenMessage( std::string message )
{
	std::stringstream			ss(message);
	std::string					token;
	std::vector<std::string>	tokenArr;

	while (ss >> token)
		tokenArr.push_back(token);

	// Print tokens.
	std::cout << B_BLUE << "BotTokens:>";
	for (size_t i = 0; i < tokenArr.size(); i++)
		std::cout << "`" << tokenArr[i] << "`";
	std::cout << "<" << END << std::endl;
	return (tokenArr);
}

std::map<std::string, std::string>
	Bot::botSplitMessage( std::string delimeter, std::string message )
{
	std::map<std::string, std::string> tokens;
	size_t pos = 0;

	// CAP LS\r\nNICK gsever\r\nUSER A B C D E F G asdf\r\n
	while ((pos = message.find(delimeter)) != std::string::npos)
	{
		int posFirst = message.find(' ');

		std::string firstWord = message.substr(0, posFirst);
		// \r ve \n karakterlerini temizle // /info yazdığımızda sonunda \n bulunuyor, kaldırmak istiyorum bu yüzden ekledim.
		for (size_t i = 0; i < firstWord.size(); ++i) {
			if (firstWord[i] == '\r' || firstWord[i] == '\n') {
				firstWord.erase(i, 1);
				--i; // Karakter silindiği için i'yi azalt
			}
		}
		// Ilk kelimeyi yani komutu büyük harfe çevirir
		for (size_t i = 0; i < firstWord.size(); ++i)
			firstWord[i] = std::toupper(firstWord[i]);

		// 1. Kısım: USER 2. Kısım: A B C D E F G asdf
		//posFirst + 1; CAP'ten sonra gelene boşluğun indexi
		//pos - posFirst - 1; "CAP LS\r\n" yapısında LS'i almak için LS'in uzunluğuna bulmaya ihtiyaç var, -1 ise \r'ı almak istemiyoruz.
		if (posFirst != -1)
			tokens.insert(std::make_pair(firstWord, message.substr(posFirst + 1, pos - posFirst - 1)));
		else
			tokens.insert(std::make_pair(firstWord, "")); // nc localhost için 'TEK' argüman için kopya durumu söz konusu oluyor, bunu engellemek için eklendi. 'Message:>B-b<'
		message.erase(0, pos + delimeter.length());
	}
	// mesaj komutsuz olarak geliyor. Kontrol için yapılandırılabilir.
	if (!message.empty()) // Bilinmeyen komut ve netcat için parslama işlemini yapıyorum.
	{
		if (message[message.size() - 1] == '\n')
			return (Bot::botSplitMessage("\n", message));
		else
		{
			message.append("\n");
			return (Bot::botSplitMessage("\n", message));
			//tokens.insert(std::make_pair("UNKNOWN", message));
		}
	}
	return (tokens);
}

void	Bot::sigHandler( int signalNum )
{
	if (signalNum == SIGINT)
	{
		std::cout << "Interrupt signal found! Bot terminating..." << std::endl;
		exit(signalNum);
	}
}