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
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../includes/Server.hpp"


void	Server::cap( Client* it, std::string message )
{
	std::cout << YELLOW << "CAP" << END << std::endl;
	if (message.length() == 6 && message.compare("LS 302") == 0)
	{
		std::string response = "CAP * LS :multi-prefix sasl";
		response += "\r\n";
		std::cout << YELLOW << "Response:>" + response << "<" << std::endl;
		send(it->getFd(), response.c_str(), response.size(), 0);
	}
	if (message.length() == 17 && message.compare("REQ :multi-prefix") == 0)
	{
		std::string response = "CAP * ACK multi-prefix";
		response += "\r\n";
		std::cout << YELLOW << "Response:>" + response << "<" << std::endl;
		send(it->getFd(), response.c_str(), response.size(), 0);
	}
	if (message.length() == 3 && message.compare("END") == 0)
	{
		if (!it->getNickname().empty())
		{
			it->setRegistered();
			//std::string response = "001 " + it->getNickname() + " :Welcome to the HELL " + it->getNickname();
			//response += "\r\n";
			std::string	welcomeStr = Server::welcomeServer();
			std::string responseStr = RPL_WELCOME(it->getNickname());
			std::cout << YELLOW << "Response:>" +  responseStr << "<" << std::endl;
			welcomeStr += responseStr;
			send(it->getFd(), welcomeStr.c_str(), welcomeStr.size(), 0);
		}
	}
}

void	Server::join( Client* it , std::string message )
{
	std::cout << YELLOW << "JOIN" << END << std::endl;
	if (it->getRegistered() == true)
	{
		if (message.find("#")) // #asdf
			message.erase(0, 1); // asdf -> #'i kaldiriyoruz.
		Channel	*channel = new Channel(message, "", it);
		_channels.insert(std::make_pair(message, channel));
		std::string response = ":" + it->getUsername() + " JOIN :" + message + "\r\n";
		std::cout << YELLOW << "Response:>" + response << "<" << std::endl;
		send(it->getFd(), response.c_str(), response.size(), 0);
	}
}

void	Server::nick( Client* it, std::string message )
{
	std::cout << YELLOW << "NICK" << END << std::endl;
	if (it->getRegistered() == false)
	{
		it->setNickname(message);
	}
	else
	{
		//std::string response = "You are now known as " + message + "\r\n";
		std::string response = ":" + it->getNickname() + " NICK " + message + "\r\n";
		it->setNickname(message);
		std::cout << YELLOW << "Response:>" + response << "<" << std::endl;
		send(it->getFd(), response.c_str(), response.size(), 0);
	}
}

void	Server::privmsg( Client* it, std::string message )
{
	std::cout << YELLOW << "PRIVMSG" << END << std::endl;
	(void)it;
	std::cout << YELLOW "PRIVMSG: " + message << END << std::endl;
	//size_t pos = message.find(' ');
	//std::string target = message.substr(0, pos);
	//std::string text = message.substr(pos + 1);
	//// Örnek: ":John!john@example.com PRIVMSG #channel :Merhaba, nasılsınız?"
	//handlePrivateMessage(it->getFd(), target, text);
}

//"USER akaraca akaraca localhost :Ahmet Karaca" komutu aşağıdaki anlamı taşır:
//Kullanıcı Adı (Username): "akaraca"
//Ana Bilgisayar Adı (Hostname): "akaraca"
//Sunucu Adı (Servername): "localhost" (genellikle boş bırakılır)
//Gerçek İsim (Realname): "Ahmet Karaca"
// void	Server::user( Client* it, std::string message )
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

void Server::user(Client* it, std::string message) {
	std::cout << YELLOW << "USER" << END << std::endl;
	if (!it->getRegistered()) {
		std::istringstream iss(message);
		std::string token;
		int i = 0;
		while (std::getline(iss, token, ' ')) {
			if (i == 0) {
				it->setUsername(token);
				std::cout << "Username:>" << it->getUsername() << std::endl;
			} else if (i == 1) {
				it->setHostname(token);
				std::cout << "Hostname:>" << it->getHostname() << std::endl;
			} else if (i == 2) {
				it->setServername(token);
				std::cout << "Servername:>" << it->getServername() << std::endl;
			} else if (i == 3) {
				std::string realname;
				std::getline(iss, realname);
				it->setRealname(token + " " + realname);
				std::cout << "Realname:>" << it->getRealname() << std::endl;
				break;
			}
			i++;
		}
		Server::cap(it, "END");
	}
}

/**
 * @brief 
 * 
 * @link https://www.antionline.com/showthread.php?136933-IRC-flags
 * @link https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.5
 * @link https://irssi.org/documentation/help/mode/
 * 
 * @param message 
 */
void Server::mode( Client* it, std::string message )
{
	std::cout << YELLOW << "MODE" << END << std::endl;
	(void)it;
	(void)message;
// #define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args
	// std::string response = ": MODE " + it->getNickname() + message + "\r\n";
	// std::cout << YELLOW << "Response:>" + response << "<" << std::endl;
	// send(it->getFd(), response.c_str(), response.size(), 0);
	// it->sendMessageFd(RPL_MODE(it->getPrefix(), ));
}

void	Server::ping( Client* it, std::string message ) // OK
{
	if (!message.compare(it->getNickname()) || !message.compare(this->_host))
		it->sendMessageFd(ERR_NEEDMOREPARAMS(it->getNickname(), "PING"));
	it->sendMessageFd(RPL_PING(it->getNickname(), message));
}
