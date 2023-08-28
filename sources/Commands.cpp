/**
 * @file Commands.cpp
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
#include "../includes/Server.hpp"


void	Server::cap( Client* it, std::vector<std::string> tokenArr ) // OK
{
	std::cout << YELLOW << "CAP" << END << std::endl;

	if (!tokenArr[0].compare("CAP")
			&& !tokenArr[1].compare("LS")
			&& !tokenArr[2].compare("302"))
		it->sendMessageFd("CAP * LS :multi-prefix sasl");

	if (!tokenArr[0].compare("CAP")
			&& !tokenArr[1].compare("REQ")
			&& !tokenArr[2].compare(":multi-prefix"))
		it->sendMessageFd("CAP * ACK multi-prefix");

	if (!tokenArr[0].compare("CAP")
			&& !tokenArr[1].compare("END"))
	{
		std::cout << "ende geldik abe de " << std::endl;
		int fd = it->getFd();
		tokenArr.erase(tokenArr.begin(), tokenArr.begin() + 2);
		Server::pass(it, tokenArr);
		if (_clients.find(fd) != _clients.end())
		{
			tokenArr.erase(tokenArr.begin(), tokenArr.begin() + 2);
			Server::nick(it, tokenArr);
			// if (it->getNickname().empty())
			// {
				
			// 	it->setRegistered();
			// 	it->sendMessageFd(Server::welcomeServer() + RPL_WELCOME(it->getNickname()));
			// }
		}
	}
}

void	Server::join( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "JOIN" << END << std::endl;
	if (it->getRegistered() == true)
	{
		if (tokenArr[1].compare("#")) // #asdf
			tokenArr[1].erase(0, 1); // asdf -> #'i kaldiriyoruz.
		Channel	*channel = new Channel(tokenArr[1], tokenArr[2], it); // token[0]=JOIN, token[1]=asdf, token[2]=password
		_channels.insert(std::make_pair(tokenArr[1], channel));
		it->sendMessageFd(RPL_JOIN(it->getUsername(), tokenArr[1]));
	}
}

void	Server::nick( Client* it, std::vector<std::string> tokenArr )
{
	// `NICK``yuandre``USER``yuandre``yuandre``localhost``:Görkem``Sever`
	std::cout << YELLOW << "NICK" << END << std::endl;
	if (tokenArr[1].empty())
	{
		it->sendMessageFd(ERR_NONICKNAMEGIVEN(it->getNickname()));
	}
	else if (Server::findClientName(tokenArr[1]))
		it->sendMessageFd(ERR_NICKNAMEINUSE(it->getNickname()));
	else
	{
		it->setNickname(tokenArr[1]);
		it->sendMessageFd(RPL_NICK(it->getPrefix(), tokenArr[1]));
	}
}

void	Server::pass( Client* it, std::vector<std::string> tokenArr ) // OK
{
	// `PASS``asdf``NICK``yuandre``USER``yuandre``yuandre``localhost``:Görkem``Sever`
	std::cout << YELLOW << "PASS" << END << std::endl;
	if (!tokenArr[0].compare("PASS"))
		if (!tokenArr[1].compare(this->_password))
			it->setRegistered();
		else
		{
			tokenArr.clear();
			tokenArr.push_back("Password is wrong.");
			Server::quit(it, tokenArr);
		}
	else
	{
		tokenArr.clear();
		tokenArr.push_back("Password is empty.");
		Server::quit(it, tokenArr);
	}
}

/**
 * @brief 
 * 
 * Her quit mesajina gondermeden once tokenArr'in icerisini temizleyip;
 *  1 tane array olacak sekilde mesaji eklememiz gerekiyor.
 * 
 * @param it 
 * @param tokenArr 
 */
void	Server::quit( Client* it, std::vector<std::string> tokenArr ) // OK
{
	// :syrk!kalt@millennium.stealth.net QUIT :Gone to have lunch
	std::cout << YELLOW << "QUIT" << END << std::endl;
	Server::info();
	int	fd = it->getFd();
	for (size_t i = 0; i < _pollfds.size(); i++)
	{
		if (it->getFd() == _pollfds[i].fd)
		{
			std::cout << "getprefix -> " << it->getPrefix() << std::endl;
			it->sendMessageFd(RPL_QUIT(it->getPrefix(), tokenArr[0]));
			close(_pollfds[i].fd);
			_pollfds.erase(_pollfds.begin() + i);
			Server::removeClient(fd);
		}
	}
	_clients.erase(fd);
	Server::info();
}

void	Server::info( void )
{
	std::cout << BLUE << "------------POLL.FDS-----------------" << END << std::endl;
	for (size_t i = 0; i < this->_pollfds.size(); i++)
		std::cout << this->_pollfds[i].fd << " - ";
	std::cout << BLUE << "\n------------CLIENTS.FDS--------------" << END << std::endl;
	for (itClients it = _clients.begin(); it != _clients.end(); it++)
		std::cout << it->first << " - ";
	std::cout << BLUE << "\n------------CHANNELS.FDS-------------" << END << std::endl;
	for (itChannels it = _channels.begin(); it != _channels.end(); it++)
		std::cout << it->first << " - ";
	std::cout << BLUE << "\n-------------------------------------" << END << std::endl;
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

// void	Server::privmsg( Client* it, std::vector<std::string> tokenArr )
// {
// 	std::cout << YELLOW << "PRIVMSG" << END << std::endl;
// 	(void)it;
// 	std::cout << YELLOW "PRIVMSG: " + message << END << std::endl;
// 	//size_t pos = message.find(' ');
// 	//std::string target = message.substr(0, pos);
// 	//std::string text = message.substr(pos + 1);
// 	//// Örnek: ":John!john@example.com PRIVMSG #channel :Merhaba, nasılsınız?"
// 	//handlePrivateMessage(it->getFd(), target, text);
// }

//"USER akaraca akaraca localhost :Ahmet Karaca" komutu aşağıdaki anlamı taşır:
//Kullanıcı Adı (Username): "akaraca"
//Ana Bilgisayar Adı (Hostname): "akaraca"
//Sunucu Adı (Servername): "localhost" (genellikle boş bırakılır)
//Gerçek İsim (Realname): "Ahmet Karaca"
// void	Server::user( Client* it, std::vector<std::string> tokenArr )
// {
// 	std::cout << YELLOW << "USER" << END << std::endl;
// 	if (it->getRegistered() == false)
// 	{
// 		size_t pos = 0;
// 		int i = 0;
// 		while ((pos = message.find(' ')) != std::string::npos)
// 		{
// 			if (i == 0)
// 			{
// 				it->setUsername(message.substr(0, pos));
// 				std::cout << "Username:>" << it->getUsername() << std::endl;
// 				message.erase(0, pos + 1);
// 			}
// 			if (i == 1)
// 			{
// 				it->setHostname(message.substr(0, pos));
// 				std::cout << "Hostname:>" << it->getHostname() << std::endl;
// 				message.erase(0, pos + 1);
// 			}
// 			if (i == 2)
// 			{
// 				it->setServername(message.substr(0, pos));
// 				std::cout << "Servername:>" << it->getServername() << std::endl;
// 				message.erase(0, pos + 1);
// 			}
// 			i++;
// 			if (i == 3)
// 			{
// 				it->setRealname(message.substr(1, message.length() - 1));
// 				std::cout << "Realname:>" << it->getRealname() << std::endl;
// 				break;
// 			}
// 		}
// 		// std::string response = "Welcome to the server!";
// 		// response += "\r\n";
// 		// std::cout << YELLOW << "Response:>" + response << "<" << std::endl;
// 		// send(it->getFd(), response.c_str(), response.size(), 0);
// 		Server::cap(it, "END");
// 	}
// }

// void Server::user(Client* it, std::vector<std::string> tokenArr) {
// 	std::cout << YELLOW << "USER" << END << std::endl;
// 	if (!it->getRegistered()) {
// 		std::istringstream iss(message);
// 		std::string token;
// 		int i = 0;
// 		while (std::getline(iss, token, ' ')) {
// 			if (i == 0) {
// 				it->setUsername(token);
// 				std::cout << "Username:>" << it->getUsername() << std::endl;
// 			} else if (i == 1) {
// 				it->setHostname(token);
// 				std::cout << "Hostname:>" << it->getHostname() << std::endl;
// 			} else if (i == 2) {
// 				it->setServername(token);
// 				std::cout << "Servername:>" << it->getServername() << std::endl;
// 			} else if (i == 3) {
// 				std::string realname;
// 				std::getline(iss, realname);
// 				it->setRealname(token + " " + realname);
// 				std::cout << "Realname:>" << it->getRealname() << std::endl;
// 				break;
// 			}
// 			i++;
// 		}
// 		Server::cap(it, "END");
// 	}
// }

/**
 * @brief 
 * 
 * @link https://www.antionline.com/showthread.php?136933-IRC-flags
 * @link https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.5
 * @link https://irssi.org/documentation/help/mode/
 * 
 * @param message 
 */
// void Server::mode( Client* it, std::vector<std::string> tokenArr )
// {
// 	std::cout << YELLOW << "MODE" << END << std::endl;
// 	(void)it;
// 	(void)message;
// // #define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args
// 	// std::string response = ": MODE " + it->getNickname() + message + "\r\n";
// 	// std::cout << YELLOW << "Response:>" + response << "<" << std::endl;
// 	// send(it->getFd(), response.c_str(), response.size(), 0);
// 	// it->sendMessageFd(RPL_MODE(it->getPrefix(), ));
// }

// void	Server::ping( Client* it, std::vector<std::string> tokenArr ) // OK
// {
// 	if (!message.compare(it->getNickname()) || !message.compare(this->_host))
// 		it->sendMessageFd(ERR_NEEDMOREPARAMS(it->getNickname(), "PING"));
// 	it->sendMessageFd(RPL_PING(it->getNickname(), message));
// }






// **************************************
// IRC (Internet Relay Chat), internet üzerinden gerçek zamanlı metin tabanlı iletişim için bir protokoldür. IRC sunucuları, sunucunun ve kanallarının farklı yönlerini kontrol etmek ve yönetmek için çeşitli komut bayrakları kullanır. Bu komut bayrakları genellikle '/mode' komutuyla birlikte kullanılır ve bayrağın ayarlandığını veya ayarlanmadığını belirtmek için önünde bir artı ('+') veya eksi ('-') işareti bulunur.

// Aşağıda bazı yaygın IRC sunucusu komut bayraklarının bir listesi verilmiştir:

// - `+b` ve `-b`: Bu bayraklar, bir kullanıcı veya ana makine maskesine yasak koymak veya kaldırmak, böylece onların bir kanala katılmasını engellemek için kullanılır.
// - `+e` ve `-e`: Bu bayraklar, bir yasağın istisnasını ayarlamak veya kaldırmak için kullanılır ve bir kullanıcının veya ana bilgisayar maskesinin yasaklanmış olsa bile bir kanala katılmasına olanak tanır.
// - `+I` ve `-I`: Bu bayraklar, bir kanalın davet listesine bir kullanıcıyı veya ana bilgisayar maskesini eklemek veya kaldırmak için kullanılır; bu, yalnızca davetle olarak ayarlanmış olsa bile bu kişilerin kanala katılmasına olanak tanır.
// - `+k` ve `-k`: Bu bayraklar, kullanıcıların kanala katılabilmeleri için sağlamaları gereken kanal anahtarını (şifre) ayarlamak veya kaldırmak için kullanılır.
// - `+l` ve `-l`: Bu bayraklar bir kanalda kullanıcı limiti ayarlamak veya kaldırmak için kullanılır ve kanala katılabilecek maksimum kullanıcı sayısını kontrol eder.
// - `+m` ve `-m`: Bu bayraklar bir kanalda denetlenen modu ayarlamak veya kaldırmak için kullanılır; bu, yalnızca ses veya operatör ayrıcalıklarına sahip kullanıcıların kanala mesaj gönderebileceği anlamına gelir.
// - `+n` ve `-n`: Bu bayraklar bir kanalda harici mesaj yok modunu ayarlamak veya kaldırmak için kullanılır; bu, kanala yalnızca kanalda bulunan kullanıcıların mesaj gönderebileceği anlamına gelir.
// - `+o` ve `-o`: Bu bayraklar, bir kanaldaki kullanıcıya operatör ayrıcalıkları vermek veya bu ayrıcalıkları almak için kullanılır.
// - `+p` ve `-p`: Bu bayraklar bir kanalda özel modu ayarlamak veya kaldırmak için kullanılır; bu, kanalın sunucunun kanal listesinde gösterilmeyeceği anlamına gelir.
// - `+s` ve `-s`: Bu bayraklar bir kanalda gizli modu ayarlamak veya kaldırmak için kullanılır; bu, kanalın sunucunun kanal listesinde gösterilmeyeceği ve kullanıcıların kimlerin kanalda olduğunu göremeyeceği anlamına gelir. onlar da içinde olmadığı sürece kanal.
// - `+t` ve `-t`: Bu bayraklar bir kanalda konu kilitleme modunu ayarlamak veya kaldırmak için kullanılır; bu, yalnızca operatör ayrıcalıklarına sahip kullanıcıların kanalın konusunu değiştirebileceği anlamına gelir.