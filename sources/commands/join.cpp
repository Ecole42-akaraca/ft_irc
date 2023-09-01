#include "../../includes/Server.hpp"

// https://dd.ircdocs.horse/refs/commands/join
// channel içinde: /name veya /names kullanıcıları listeler.
void	Server::join( Client* it, std::vector<std::string> tokenArr ) // kullanici diğer kullanicinin katildiğini görmüyor.
{
	std::cout << YELLOW << "JOIN" << END << std::endl;
	if (it->getRegistered() == false)
		return ;
	if (!tokenArr[1].compare("#")) // asdf
		tokenArr[1] = "#" + tokenArr[1]; // #asdf -> #'i ekliyoruz.

	itChannels find = _channels.find(tokenArr[1]); // aynı isimde kanal var mı?
	it->sendMessageFd(RPL_JOIN(it->getPrefix(), tokenArr[1])); // kanal olsada olmasada katılması gerekiyor.
	if (find != _channels.end()) // Channel mevcut mu?
	{
		this->_channels[tokenArr[1]]->sendMessageBroadcast(\
			it, RPL_JOIN(it->getPrefix(), tokenArr[1]));
		it->sendMessageFd(RPL_TOPIC(it->getPrefix(), tokenArr[1], "New User Login Channel!")); // Kullanıcı kanala katıldığı zaman bilgi paylaşıyor.
		find->second->addClient(it); // var olan channel'a yeni kullanıcıyı ekle.
		for (size_t i = 0; i < find->second->_channelClients.size(); i++) // kullanıcı var olan bir kanala girdiği zaman, kanaldaki kullancıları listele.
		{
			std::string pref = "";
			if (find->second->_channelClients[i]->getNickname().compare(find->second->getAdmin()->getNickname()) == 0)
				pref = "@";
			it->sendMessageFd(RPL_NAMREPLY(it->getNickname(), tokenArr[1], pref + find->second->_channelClients[i]->getNickname()));
		}
		it->sendMessageFd(RPL_ENDOFNAMES(it->getPrefix(), tokenArr[1]));
	}
	else // Channel mevcut değilse, yeni channel için ayarlama yap.
	{
		Channel *channel = new Channel(tokenArr[1], "NULL", it); // channel oluştur. // channel'e admin ekle.
		//channel->setAdmin(it); // channel'e admin ekle.
		channel->addClient(it); // Admini channel'in _channelClient'ına ekliyor.
		this->_channels.insert(std::make_pair(tokenArr[1], channel)); // Server'a channel'ı ekle.
		it->sendMessageFd(RPL_TOPIC(it->getPrefix(), tokenArr[1], "New Channel Created.")); // Kullanıcı kanala katıldığı zaman bilgi paylaşıyor.
		it->sendMessageFd(RPL_MODE(it->getNickname(), tokenArr[1], "+nto", it->getNickname())); // Kimlik doğrulaması ve admini belirt.
	}
}