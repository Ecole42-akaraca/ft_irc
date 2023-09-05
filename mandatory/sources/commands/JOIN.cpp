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
	if (tokenArr[1][0] != '#') // '/join asdf' şeklinde kanal oluşturmak için isim belirlendiğinde,
		tokenArr[1] = "#" + tokenArr[1];  // Girilen ismin sonuna '#' ekleniyor, Bunun nedeni channel isimleri '#' ile başlamaktadır.

	itChannels itChannel = _channels.find(tokenArr[1]); // Aynı isimde channel var mı kontrol ediliyor.
	it->sendMessageFd(RPL_JOIN(it->getPrefix(), tokenArr[1])); // Kullanıcı kanal olsada, olmasada hertürlü o kanala katılacağından dolayı RPL yanıtını gönderiyoruz.
	if (itChannel != _channels.end()) // Channel mevcutsa buraya giriyor.
	{
		if (itChannel->second->getPassword().compare(tokenArr[2]) != 0)
		{
			it->sendMessageFd(ERR_PASSWDMISMATCH(it->getPrefix())); // Channela girerken şifresi yanlışsa
			it->sendMessageFd(RPL_KICK(it->getPrefix(), tokenArr[1], tokenArr[1], "Password is incorrect!")); // Irc client channel oluşturuyor lakin boş gözüküyor, bu yüzden kicklemeliyiz.
			return ;
		}
		if (itChannel->second->getMaxClient() != -1 &&\
		 itChannel->second->getMaxClient() < itChannel->second->getClientCount() + 1)
		{
			it->sendMessageFd(ERR_CHANNELISFULL(it->getPrefix(), tokenArr[1]));
			return ;
		}
		itChannel->second->sendMessageBroadcast(it, RPL_JOIN(it->getPrefix(), tokenArr[1])); // Channel'da bulunan diğer Client'lara yeni katılan Client'ın katıldı bilgisi gönderiliyor.
		itChannel->second->setChannelTopic("I have no idea what the topic is.");
		it->sendMessageFd(RPL_TOPIC(it->getPrefix(), tokenArr[1], itChannel->second->getChannelTopic())); // var olan bir Channel'e kullanıcı katıldığı zaman göreceği channel başlığı bilgisi gönderiliyor.
		itChannel->second->addClient(it); // Channel'e bağlı olan client'lar listesi güncelleniyor.
		itChannel->second->channelUsers(it, itChannel->second, tokenArr[1]); // Client, channel'e bağlandığı zaman, channel'deki kullanıcılar listeleniyor.
		it->registerChannel(itChannel->second); // Client içinde bulunan, kayıtlı kullanıcıların olduğu listeye channel ekleniyor.
	}
	else // Channel mevcut değilse, yeni channel oluşturulup, ayarlamalar gerçekleştiriliyor.
	{
		Channel *channel = new Channel(tokenArr[1], "", it); // Channel oluşturulur ve admini belirlenir. Channel şifresi için henüz bir şey yoktur.
		channel->addClient(it); // Channel'ı oluşturan kişiyi _channelClient'ına ekliyor.
		this->_channels.insert(std::make_pair(tokenArr[1], channel)); // Server'a channel'ı ekliyor.
		channel->setChannelTopic("What day is it today?"); // Channel başlığı belirleniyor.
		it->sendMessageFd(RPL_TOPIC(it->getPrefix(), tokenArr[1], channel->getChannelTopic())); // Client kanala katıldığı zaman channel başlığı gönderiliyor.
		it->sendMessageFd(RPL_MODE(it->getNickname(), tokenArr[1], "+nto", it->getNickname())); // Kimlik doğrulaması gerçekleşiyor.
		it->registerChannel(channel); // Client içinde bulunan, kayıtlı kullanıcıların olduğu listeye channel ekleniyor.
	}
}

/*
+o: Op (Operator) modu, kullanıcının kanalda yönetici veya moderatör olmasını sağlar. Yani, kullanıcı kanalı yönetme yetkisine sahip olur.

+v: Voice modu, kullanıcının kanalda sesli konuşma yetkisine sahip olmasını sağlar. Diğer kullanıcılar, sesi olan kullanıcıları daha iyi duyar.

+n: No External Messages modu, kanala katılan kullanıcıların özel mesajlarına kısıtlamalar getirir, böylece yalnızca kanalda açıkça konuşulan mesajları görebilirler.

+t: Topic (Konu) modu, sadece kanal operatörlerinin kanalın konusunu (topic) değiştirmesine izin verir.

+m: Moderated modu, sadece kanal operatörleri veya sesi olan kullanıcıların mesaj göndermesine izin verir.
*/