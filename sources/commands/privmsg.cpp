#include "../../includes/Server.hpp"

/*
// in the channel: Message:>PRIVMSG-#ASDF :Ben deli değilim.<
	tokenArr[0] -> Command Name -> "PRIVMSG" 
	tokenArr[1] -> Channel Name -> #ASDF
	tokenArr[2] -> First Message -> :Ben
	tokenArr[3] -> Second Message -> deli
	tokenArr[4] -> Third Message -> değilim.
-> Channel dışında, "/msg #ASDF Ben deli değilim." şeklindeki kullanımı, channel'e mesaj gönderecektir.

// out the channel: Message:>PRIVMSG-akaraca :Ben deli değilim.<
	tokenArr[0] -> Command Name -> "PRIVMSG" 
	tokenArr[1] -> Nick Name -> akaraca
	tokenArr[2] -> First Message -> :Ben
	tokenArr[3] -> Second Message -> deli
	tokenArr[4] -> Third Message -> değilim.

*/
void	Server::privmsg( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "PRIVMSG" << END << std::endl;

	std::string	msg;
	tokenArr[2].erase(0, 1); 	// token[2] token2'ni basindaki : kaldırır
	for (size_t i = 2; i < tokenArr.size(); i++)
	{
		if (i + 1 < tokenArr.size())
			msg += tokenArr[i] + " ";
		else
			msg += tokenArr[i];
	}
	if (tokenArr[1][0] == '#') // Channel içinde gönderilen mesajı temsil ediyor.
	{
		if (_channels.find(tokenArr[1]) != _channels.end()) // belirtilen isimde channel varsa,
		{
			this->_channels[tokenArr[1]]->sendMessageBroadcast(\
				it, RPL_PRIVMSG(it->getPrefix(), tokenArr[1], msg));
			// Örnek: ":John!john@example.com PRIVMSG #channel :Merhaba, nasılsınız?"
		}
		else // belirtilen isimde channel yoksa hata döndürmeliyiz.
			it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[1]));
	}
	else //Channel dışında doğrudan kullanıcıya atılan mesajı temsil ediyor.
	{
		int fd = Server::findClientName(tokenArr[1]);
		if (fd != -1)
		{
			Client *destClient = _clients.find(fd)->second;
			destClient->sendMessageFd(RPL_PRIVMSG(it->getPrefix(), tokenArr[1], msg));
			// Mesaj göndereceğimiz client'tı bulup onun client'ında sendMessageFd'yi çalıştırıyoruz.
			// Eğer kendi mesajı gönderenin clien'tinden yaparsak, 2 mesaj kendisine göndermiş olacak, çünkü IRC client zaten komutu girdiğimiz anda kendisinide yazmaktadır.
		}
		else // kişi yoksa hata döndürmelidir.
			it->sendMessageFd(ERR_NOSUCHNICK(it->getPrefix(), tokenArr[1]));
	}
}