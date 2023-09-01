#include "../../includes/Server.hpp"

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

	for (itChannels itC = _channels.begin(); itC != _channels.end(); itC++) // Çıkan kullanıcı hangi kanaldaysa o kanaldan ayrıldığı bilgisini vermelidir.
	{
		for (size_t i = 0; i < itC->second->_channelClients.size(); i++)
		{
			if (itC->second->_channelClients[i]->getNickname().compare(it->getNickname()) == 0)
			{
				std::vector<std::string> msg;
				msg.push_back("PART");
				msg.push_back(itC->second->getName());
				msg.push_back(":QUIT");
				Server::part(it, msg);
			}
		}
	}

	Server::infoFd();
	int	fd = it->getFd();
	for (size_t i = 0; i < _pollfds.size(); i++)
	{
		if (it->getFd() == _pollfds[i].fd)
		{
			it->setActiveStatus();
			it->sendMessageFd(RPL_QUIT(it->getPrefix(), tokenArr[0]));
			close(_pollfds[i].fd);
			_pollfds.erase(_pollfds.begin() + i);
			Server::removeClient(fd);
			return ;
		}
	}
	_clients.erase(fd);
	Server::infoFd();
}