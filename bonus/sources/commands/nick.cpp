# include "../../includes/Server.hpp"

/*
	// `NICK``yuandre``USER``yuandre``yuandre``localhost``:Görkem``Sever`
*/
void	Server::nick( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "NICK" << END << std::endl;
	if (tokenArr.size() < 2 || tokenArr[1][0] == '#') // saçma bir nick girme işlemi gerçekleşirse önlemek için. // kullanıcı adının başında # olamaz.
	{
		std::string name = "";
		if (tokenArr.size() > 1) // Boş nick'li biri kayıt olmaya çalıştığı zaman seg yiyor, bunu önlemek için parametre sayısı kontrolü yapıyoruz, bu kısım ayrıca #'li kullanıcı adlarını belirtmek için bulunmaktadır.
			name = tokenArr[1];
		it->sendMessageFd(ERR_ERRONEUSNICKNAME(it->getPrefix(), name)); // Client'e uygun olmayan kullanıcı adına sahip olduğu bilgisini gönderiyoruz. Kayıtlı kullanıcıları ise sadece uyarmak için vardır.
		if (it->getIRCstatus() != AUTHENTICATED) // Client'in ilk bağlantısı için hatalı isim kullanan Client'ler doğrudan server'dan atılmaktadır. 
			Server::quitReason(it, "Nickname is wrong use!"); // Client'e Quit atarken bir neden belirtmek gerekiyor.
		return ;
	}

	if (tokenArr[1].empty())
		it->sendMessageFd(ERR_NONICKNAMEGIVEN(tokenArr[1])); // Client '/nick ' şeklinde bir ibare kullanırsa
	else if (Server::getClientFdByNickname(tokenArr[1]) != -1) // Aynı isme sahip bir client varsa -1 döndürür.
		it->sendMessageFd(ERR_NICKNAMEINUSE(tokenArr[1])); // Client, server'a kayıtlı aynı nick'e sahip bir kullanıcı varsa, bu ismi alamayacağını belirtmek için vardır.
	else
	{
		if (it->getIRCstatus() == AUTHENTICATED) // Sadece kayıtlı kullancıların erişebileceği, nick değiştirme yeridir.
		{
			it->sendMessageFd(RPL_NICK(it->getPrefix(), tokenArr[1])); // Client'e kullanıcı adının değiştiği bilgisi verilir.
			for (size_t i = 0; i < it->getRegisteredChannels().size(); ++i)
				it->getRegisteredChannels()[i]->sendMessageBroadcast(it, RPL_NICK(it->getPrefix(), tokenArr[1])); // Bağlı olduğu tüm channelere isminin değiştiği bilgisi gidecek.
		}
		else // İlk client bağlantısı için nick belirlemek için kullanılmaktadır.
		{
			it->sendWelcomeMessage(Server::welcomeServer()); // ilk bağlantı olduğundan dolayı, emoji mesajıdır
			it->sendMessageFd(RPL_WELCOME(tokenArr[1], _serverName));  // ilk bağlantı olduğundan dolayı, selamlama mesajıdır
			if (it->getPasswordStatus() == true) // re_nick durumunda yaşanan problemi atlamak için koydum.
			{
				it->setIRCstatus(AUTHENTICATED); // Ilk bağlantı ile güvenli bir şekilde nick belirlendiyse kullanıcının kimlik doğrulaması ok sayılır.
				it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client status is now: AUTHENTICATED."));
			}
		}
		it->setNickname(tokenArr[1]); // RPL_NICK kısmında eski kullanıcı adına ihtiyaç olduğundan dolayı (hangi kullanıcın adı, ne oldu?) en sona client'in nick'ini belirleme eklendi.
	}
}

