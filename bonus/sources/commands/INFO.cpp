# include "../../includes/Server.hpp"

void	Server::info( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "INFO" << END << std::endl;
	(void)tokenArr;
	it->sendMessageFd(RPL_INFO(it->getPrefix(), std::string("Bu ft_irc projesi, akaraca ve gsever tarafından oluşturulmuştur.")));
	it->sendMessageFd(RPL_INFO(it->getPrefix(), std::string("Serverdaki tüm kullanıcılar:")));
	for (itClients itC = _clients.begin(); itC != _clients.end(); ++itC)
	{
		Client *dest = itC->second;
		std::stringstream ss;
		ss << dest->getPort();
		it->sendMessageFd(RPL_WHOISUSER(it->getPrefix(), dest->getNickname(), dest->getUsername(), dest->getHostname(), ss.str(), dest->getRealname()));
	}
	it->sendMessageFd(RPL_ENDOFWHOIS(it->getPrefix(), ""));

	it->sendMessageFd(RPL_ENDOFINFO(it->getPrefix()));
}
