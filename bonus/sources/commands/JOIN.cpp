# include "../../includes/Server.hpp"

/*
// https://dd.ircdocs.horse/refs/commands/join
// channel içinde: /name veya /names kullanıcıları listeler.
// Var olan bir channele biri bağlandığında mesaj bastırmalıdır : 'Guest46147 [~AHMET@176.88.10.20] has joined #42Kocaeli'
*/
void	Server::join( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "JOIN" << END << std::endl;

	if (it->getIRCstatus() != AUTHENTICATED) // Kayıt işlemi bitmemiş Client'ler join fonksiyonuna girmesin istiyoruz, irssi client kayıt işlemini bitirmeden JOIN komutu gönderiyor.
	{
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
		return ;
	}

	if (tokenArr.at(1)[0] != '#') // '/join asdf' şeklinde kanal oluşturmak için isim belirlendiğinde,
		tokenArr[1] = "#" + tokenArr[1];  // Girilen ismin sonuna '#' ekleniyor, Bunun nedeni channel isimleri '#' ile başlamaktadır.

	itChannels itChannel = _channels.find(tokenArr[1]); // Aynı isimde channel var mı kontrol ediliyor.
	//it->sendMessageFd(RPL_JOIN(it->getPrefix(), tokenArr[1])); // Kullanıcı kanal olsada, olmasada hertürlü o kanala katılacağından dolayı RPL yanıtını gönderiyoruz.
	if (itChannel != _channels.end()) // Channel mevcutsa buraya giriyor.
	{
		if (!itChannel->second->getPassword().empty() &&\
			((tokenArr.size() != 3 ) || (tokenArr.size() == 3 && itChannel->second->getPassword().compare(tokenArr[2]) != 0))) // eğer channelin şifresi varsa
		{
			it->sendMessageFd(ERR_PASSWDMISMATCH(it->getPrefix())); // Channela girerken şifresi yanlışsa
			it->sendMessageFd(RPL_KICK(it->getPrefix(), tokenArr[1], tokenArr[1], "Password is incorrect!")); // Irc client channel oluşturuyor lakin boş gözüküyor, bu yüzden kicklemeliyiz.
			return ;
		}

		if (itChannel->second->getMaxClient() != 0 &&\
		 itChannel->second->getMaxClient() < itChannel->second->getClientCount() + 1)
		{
			it->sendMessageFd(ERR_CHANNELISFULL(it->getPrefix(), tokenArr[1]));
			return ;
		}

		itChannel->second->sendMessageBroadcast(it, RPL_JOIN(it->getPrefix(), tokenArr[1])); // Channel'da bulunan diğer Client'lara yeni katılan Client'ın katıldı bilgisi gönderiliyor.
		itChannel->second->addClient(it); // Channel'e bağlı olan client'lar listesi güncelleniyor.
		itChannel->second->channelUsers(it, itChannel->second, tokenArr[1]); // Client, channel'e bağlandığı zaman, channel'deki kullanıcılar listeleniyor.
		it->registerChannel(itChannel->second); // Client içinde bulunan, kayıtlı kullanıcıların olduğu listeye channel ekleniyor.
		it->sendMessageFd(RPL_JOIN(it->getPrefix(), tokenArr[1])); // Kullanıcı kanal olsada, olmasada hertürlü o kanala katılacağından dolayı RPL yanıtını gönderiyoruz.
		it->sendMessageFd(RPL_TOPIC(it->getPrefix(), tokenArr[1], itChannel->second->getChannelTopic())); // var olan bir Channel'e kullanıcı katıldığı zaman göreceği channel başlığı bilgisi gönderiliyor.
	}
	else // Channel mevcut değilse, yeni channel oluşturulup, ayarlamalar gerçekleştiriliyor.
	{
		// std::vector<Client*>	newAdmins;
		Channel *channel = new Channel(tokenArr[1], "\0", it); // Channel oluşturulur ve admini belirlenir. Channel şifresi için henüz bir şey yoktur.
		channel->addClient(it); // Channel'ı oluşturan kişiyi _channelClient'ına ekliyor.
		channel->setChannelMods("nt");
		if (it->getNickname().compare("ircBot") // Bu Channel'i Bot kendisi olusturursa 2 kere Botu eklemeyi engellemek icin.
				&& this->getClientByNickname("ircBot") != NULL) // Gercekten de Bot mu?
		{
			std::cout << "Bot eklendi" << std::endl;
			channel->addClient(this->getClientByNickname("ircBot")); // Ikinci Client olarak Bot'u ekle.
			channel->addAdmin(this->getClientByNickname("ircBot"));
			// this->getClientByNickname("ircBot")->sendMessageFd(RPL_JOIN(this->getClientByNickname("ircBot")->getPrefix(), tokenArr[1])); // Kullanıcı kanal olsada, olmasada hertürlü o kanala katılacağından dolayı RPL yanıtını gönderiyoruz.
			// this->getClientByNickname("ircBot")->sendMessageFd(RPL_MODE(this->getClientByNickname("ircBot")->getNickname(), tokenArr[1], "+nto", this->getClientByNickname("ircBot")->getNickname())); // Kimlik doğrulaması gerçekleşiyor.
		}
		// channel->setAdmin(this->getClientByNickname("ircBot"));
		// newAdmins.push_back(it);
		// newAdmins.push_back(this->getClientByNickname("ircBot"));
		// channel->setAdmins(newAdmins);
		this->_channels.insert(std::make_pair(tokenArr[1], channel)); // Server'a channel'ı ekliyor.
		channel->setChannelTopic("What day is it today?"); // Channel başlığı belirleniyor.
		it->registerChannel(channel); // Client içinde bulunan, kayıtlı kullanıcıların olduğu listeye channel ekleniyor.
		it->sendMessageFd(RPL_JOIN(it->getPrefix(), tokenArr[1])); // Kullanıcı kanal olsada, olmasada hertürlü o kanala katılacağından dolayı RPL yanıtını gönderiyoruz.
		it->sendMessageFd(RPL_TOPIC(it->getPrefix(), tokenArr[1], channel->getChannelTopic())); // Client kanala katıldığı zaman channel başlığı gönderiliyor.
		it->sendMessageFd(RPL_MODE(it->getNickname(), tokenArr[1], "+o" + channel->getChannelMods(), "")); // Channel'in modları atanıyor
	}
}

/*
+o: Op (Operator) modu, kullanıcının kanalda yönetici veya moderatör olmasını sağlar. Yani, kullanıcı kanalı yönetme yetkisine sahip olur.

+v: Voice modu, kullanıcının kanalda sesli konuşma yetkisine sahip olmasını sağlar. Diğer kullanıcılar, sesi olan kullanıcıları daha iyi duyar.

+n: No External Messages modu, kanala katılan kullanıcıların özel mesajlarına kısıtlamalar getirir, böylece yalnızca kanalda açıkça konuşulan mesajları görebilirler.

+t: Topic (Konu) modu, sadece kanal operatörlerinin kanalın konusunu (topic) değiştirmesine izin verir.

+m: Moderated modu, sadece kanal operatörleri veya sesi olan kullanıcıların mesaj göndermesine izin verir.
*/
