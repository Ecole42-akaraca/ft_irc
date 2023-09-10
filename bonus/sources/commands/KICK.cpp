# include "../../includes/Server.hpp"

/*
	'KICK #channel nickname :message'
	'KICK #TEST akaraca :Get of my channel'
	'/kick akaraca Get of my channel'
		or
	'/kick #42 akaraca Get of my channel'
*/
void	Server::kick( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "KICK" << END << std::endl;

	if (it->getIRCstatus() != AUTHENTICATED)
	{
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
		return ;
	}

	itChannels itChan = _channels.find(tokenArr.at(1));
	if (itChan != _channels.end()) // Belirtilen channel var mıdır?
	{
		if (isChannelAdmin(it, itChan->second)) // /kick komutunu kullanan kişi o channelin admini mi? (channel'de user mi diye kontrol etmeye gerek yok, zaten admin kontrolü ile sağlanmış olunuyor.)
		{
			Client* user = getClientByNickname(tokenArr.at(2));
			if (user != NULL)
			{
				if (isChannelUser(user, itChan->second)) // Channel'dan atılmak istenen kişi var mıdır?
				{
					itChan->second->sendMessageBroadcast(RPL_KICK(it->getPrefix(), tokenArr[1], tokenArr[2], combineMessage(3, tokenArr))); // kicklenen kişinin tüm channel'le duyurulması için ve kickleyen kişide işe yaraması için
					user->unregisterChannel(itChan->second); // Client'a ait kayıtlı olunan channeler'dan, channel'ı kaldırıyorum.
					itChan->second->removeClient(user); // Channel'a kayıtlı kullanıcılardan client'i kaldırıyorum.
					if (itChan->second->getClientCount() <= 1)
					{
						std::cout << "Channel'deki client sayisi: " << itChan->second->getClientCount() << std::endl;
						if (itChan->second->getClientCount() == 0) // Channel'de kimse kalmadiysa Channel'i kapat.
							Server::removeChannel(itChan->second->getName());
						else if (itChan->second->searchClient(this->getClientByNickname("ircBot")) != NULL) // Channel'de kalan son 2 kisiden birisi 'ircBot'sa Channel'i kapat.
							Server::removeChannel(itChan->second->getName());
						// Buraya kadar geldiyse demek ki bot olmadan 2 ya da daha cok kisi var Channel'de.
					}
				}
				else
					it->sendMessageFd(ERR_USERNOTINCHANNEL(it->getPrefix(), tokenArr[2], tokenArr[1])); // Channel'de atılacak kişi yoksa hata döndürülür.
			}
			else
				it->sendMessageFd(ERR_NOSUCHNICK(it->getPrefix(), tokenArr[2])); // Atılacak kullanıcı yoksa hata döndürülür.
		}
		else
			it->sendMessageFd(ERR_CHANOPRIVSNEEDED(it->getPrefix(), tokenArr[1]));
	}
	else
		it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[1])); // Channel yoksa hata döndürülür.
}
