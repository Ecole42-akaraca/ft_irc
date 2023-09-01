#include "../../includes/Server.hpp"

/**
 * @brief 
 * 
 * Irssi -> IRC Server
 * Eger part komutu Irssi tarafindan ana dizindeyse;
 * 	/part 			-> 12:03 -!- Irssi: Not enough parameters given
 * 	/part asdf		-> Message:>PART-asdf<
 * Eger part komutunu Channel'in icerisinde yazarsak;
 * 	/part 			-> Message:>PART-#asdf<
 * 	/part See ya	-> Message:>PART-#asdf :See ya<
 * 
 * @param it 
 * @param tokenArr 
 */
void	Server::part( Client* it, std::vector<std::string> tokenArr )
{
	// Message:>PART-#asdf :Lorem ipsum naber ehe.<
	std::cout << YELLOW << "PART" << END << std::endl;
	if (_channels.find(tokenArr[1]) != _channels.end()) // Channel varsa eğer,
	{
		std::string	msg;
		tokenArr[2].erase(0, 1); 	// token[2] token2'ni basindaki : kaldırır
		for (size_t i = 2; i < tokenArr.size(); i++)
		{
			if (i + 1 < tokenArr.size())
				msg += tokenArr[i] + " ";
			else
				msg += tokenArr[i];
		}
		// kanaldan çıktığına dair herhangi bir bilgi paylaşmıyor (kendisine)
		// Ama ayrıldığı channele mesaj yazdırmalıdır: 'BEN_ [~AHMET@176.88.10.20] has left #42kocaeli []'
		it->sendMessageFd(RPL_PART(it->getPrefix(), tokenArr[1], msg)); // Kanaldan ayrılması için.
		this->_channels[tokenArr[1]]->sendMessageBroadcast(\
				it, RPL_PART(it->getPrefix(), tokenArr[1], msg)); // Kanaldaki tüm kullancıların görmesi için.
		_channels[tokenArr[1]]->removeClient(it); // Channel'den client çıkınca client'inin silinmesi gerek.
		if (_channels[tokenArr[1]]->getClientCount() == 0) // eğerki channelde kimse kalmazsa silinsin.
			Server::removeChannel(_channels[tokenArr[1]]->getName());
	}
	else // eğerki kanal yoksa hata döndürmelidir
	{
		it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[1]));
	}

	// Eger part komutu ile channel'den ayrildiginda; channel'de kullanici
	// 	kalmadiginda channel kapatilir. Bunun kontrolu quit komutu icinde
	// 	kontrol edilmeli.
}
