# include "../../includes/Server.hpp"

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

// Örnek: ":John!john@example.com PRIVMSG #channel :Merhaba, nasılsınız?"

*/
void	Server::privmsg( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "PRIVMSG" << END << std::endl;

	if (it->getIRCstatus() != AUTHENTICATED)
	{
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
		return ;
	}

	std::string msg = Server::combineMessage(2, tokenArr);
	if (tokenArr.at(1)[0] == '#') // Channel içinde gönderilen mesajı temsil ediyor.
	{
		if (it->isRegisteredChannel(tokenArr[1]) == true)  // belirtilen isimde channel varsa,
			this->_channels[tokenArr[1]]->sendMessageBroadcast(it, RPL_PRIVMSG(it->getPrefix(), tokenArr[1], msg));
		else // belirtilen isimde channel yoksa hata döndürmeliyiz.
			it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[1]));
	}
	else //Channel dışında doğrudan kullanıcıya atılan mesajı temsil ediyor.
	{
		Client *destClient = Server::getClientByNickname(tokenArr[1]);
		if (destClient != NULL)
		{
			// PRIVMSG TEST :!send ./Desktop/1.txt
			// PRIVMSG TEST :!get 1.txt
			// irssi client'te /dcc komutu kendiliğinden çalıştığından dolayı, irssi için privmsg'de ele alıyoruz ve dcc'ye yönlendiriyoruz.
			if ((tokenArr.at(2).compare(":!send") == 0 || tokenArr.at(2).compare(":!get") == 0) && !tokenArr.at(3).empty())
			{
				std::vector<std::string> msgDCC;
				msgDCC.push_back("DCC");
				msgDCC.push_back((tokenArr[2].compare(":!send") == 0 ? "SEND" : "GET"));
				msgDCC.push_back(tokenArr[1]); // <nick_name>
				msgDCC.push_back(tokenArr[3]); // <file>
				Server::dcc(it, msgDCC);
			}
			else
				destClient->sendMessageFd(RPL_PRIVMSG(it->getPrefix(), tokenArr[1], msg));
			// Mesaj göndereceğimiz client'tı bulup onun client'ında sendMessageFd'yi çalıştırıyoruz.
			// Eğer kendi mesajı gönderenin clien'tinden yaparsak, 2 mesaj kendisine göndermiş olacak, çünkü IRC client zaten komutu girdiğimiz anda kendisinide yazmaktadır.
		}
		else  // kişi yoksa hata döndürmelidir.
			it->sendMessageFd(ERR_NOSUCHNICK(it->getPrefix(), tokenArr[1]));
	}
}
