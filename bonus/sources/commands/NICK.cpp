# include "../../includes/Server.hpp"

/*
	// `NICK``yuandre``USER``yuandre``yuandre``localhost``:Görkem``Sever`
*/
void	Server::nick( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "NICK" << END << std::endl;

	if (it->getIRCstatus() == HANDSHAKE && tokenArr.size() == 2)
	{
		it->setNickname(tokenArr[1]);
		return ;
	}

	if (it->getIRCstatus() != AUTHENTICATED && it->getIRCstatus() != RENICK)
	{
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
		return;
	}

	if (tokenArr[1].empty())
		it->sendMessageFd(ERR_NONICKNAMEGIVEN(tokenArr[1])); // Client '/nick ' şeklinde bir ibare kullanırsa
	else if (tokenArr.size() < 2 || tokenArr[1][0] == '#') // saçma bir nick girme işlemi gerçekleşirse önlemek için. // kullanıcı adının başında # olamaz.
	{
		std::string name = "";
		if (tokenArr.size() > 1) // Boş nick'li biri kayıt olmaya çalıştığı zaman seg yiyor, bunu önlemek için parametre sayısı kontrolü yapıyoruz, bu kısım ayrıca #'li kullanıcı adlarını belirtmek için bulunmaktadır.
			name = tokenArr[1];
		it->sendMessageFd(ERR_ERRONEUSNICKNAME(it->getPrefix(), name)); // Client'e uygun olmayan kullanıcı adına sahip olduğu bilgisini gönderiyoruz. Kayıtlı kullanıcıları ise sadece uyarmak için vardır.
	}
	else if (Server::getClientFdByNickname(tokenArr[1]) != -1) // Aynı isme sahip bir client varsa -1 döndürür.
		it->sendMessageFd(ERR_NICKNAMEINUSE(tokenArr[1])); // Client, server'a kayıtlı aynı nick'e sahip bir kullanıcı varsa, bu ismi alamayacağını belirtmek için vardır.
	else
	{
		if (it->getIRCstatus() == AUTHENTICATED) // Sadece kayıtlı kullancıların erişebileceği, nick değiştirme yeridir.
		{
			it->sendMessageFd(RPL_NICK(it->getPrefix(), tokenArr[1])); // Client'e kullanıcı adının değiştiği bilgisi verilir.
			for (size_t i = 0; i < it->getRegisteredChannels().size(); ++i)
				it->getRegisteredChannels()[i]->sendMessageBroadcast(it, RPL_NICK(it->getPrefix(), tokenArr[1])); // Bağlı olduğu tüm channelere isminin değiştiği bilgisi gidecek.
			it->setNickname(tokenArr[1]); // RPL_NICK kısmında eski kullanıcı adına ihtiyaç olduğundan dolayı (hangi kullanıcın adı, ne oldu?) en sona client'in nick'ini belirleme eklendi.
		}
		else if (it->getIRCstatus() == RENICK)// ilk bağlantıdaki client'ın adı aynı ise buraya girer.
		{
			it->sendWelcomeMessage(Server::welcomeServer()); // ilk bağlantı olduğundan dolayı, emoji mesajıdır
			it->sendMessageFd(RPL_WELCOME(tokenArr[1], _serverName));  // ilk bağlantı olduğundan dolayı, selamlama mesajıdır
			it->setNickname(tokenArr[1]);
			if (it->getPasswordStatus() == true) // re_nick durumunda yaşanan problemi atlamak için koydum.
			{
				it->setIRCstatus(AUTHENTICATED); // Ilk bağlantı ile güvenli bir şekilde nick belirlendiyse kullanıcının kimlik doğrulaması ok sayılır.
				it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client status is now: AUTHENTICATED."));
			}
		}
	}
}

//END -> NICK -> USER
//END -> NICK -> USER -> NICK

//END -> USER => NİCK KONTROL -> EĞER VARSA HATA DÖNDÜR -> NICK
