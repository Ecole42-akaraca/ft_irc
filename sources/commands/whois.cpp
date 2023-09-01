#include "../../includes/Server.hpp"

/*

		311     RPL_WHOISUSER
						"<nick> <user> <host> * :<real name>"
		312     RPL_WHOISSERVER
						"<nick> <server> :<server info>"
		313     RPL_WHOISOPERATOR
						"<nick> :is an IRC operator"
		317     RPL_WHOISIDLE
						"<nick> <integer> :seconds idle"
		318     RPL_ENDOFWHOIS
						"<nick> :End of /WHOIS list"

	BUNLARIN HEPSİ BURADA GÖNDERİLMELİ
*/
void	Server::whois( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "WHOIS" << END << std::endl;
	int fd = Server::findClientName(tokenArr[1]);
	if (fd != -1)
	{
		Client *dest = _clients.at(fd);
		it->sendMessageFd(RPL_WHOISUSER(it->getNickname(), dest->getNickname(), dest->getUsername(), dest->getHostname(), dest->getRealname()));
		it->sendMessageFd(RPL_ENDOFWHOIS(it->getNickname(), dest->getNickname()));
	}
	else
		it->sendMessageFd(ERR_NOSUCHNICK(it->getNickname(), tokenArr[1])); // bu hata mesajı sonrasında whowas döndürüyor
}