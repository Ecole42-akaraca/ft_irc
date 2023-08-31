/**
 * @file Commands.cpp
 * @author Görkem SEVER (gsever), Ahmet KARACA (akaraca)
 * @brief 
 * @version 0.1
 * @date 2023-08-27
 * 
 * @note: How messaging 2 client each other?
 * @link http://chi.cs.uchicago.edu/chirc/irc_examples.html
 * 
 * @note: All IRC commands.
 * @link http://www.ae.metu.edu.tr/~evren/history/documents/commands.htm
 * @link http://www.csun.edu/~webteach/mirc/commands.html
 * @link https://www.antionline.com/showthread.php?136933-IRC-flags
 * @link https://modern.ircdocs.horse/#irc-concepts
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../includes/Server.hpp"

void	Server::cap( Client* it, std::vector<std::string> tokenArr ) // OK
{
	std::cout << YELLOW << "CAP" << END << std::endl;

	if (!tokenArr[0].compare("CAP")
			&& !tokenArr[1].compare("LS")
			&& !tokenArr[2].compare("302"))
		it->sendMessageFd("CAP * LS :multi-prefix sasl");

	if (!tokenArr[0].compare("CAP")
			&& !tokenArr[1].compare("REQ")
			&& !tokenArr[2].compare(":multi-prefix"))
		it->sendMessageFd("CAP * ACK multi-prefix");

	if (!tokenArr[0].compare("CAP")
			&& !tokenArr[1].compare("END"))
	{
		if (!it->getNickname().empty())
		{
			it->setRegistered();
			it->sendMessageFd(Server::welcomeServer() + RPL_WELCOME(it->getNickname()));
		}
	}
}

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

void	Server::nick( Client* it, std::vector<std::string> tokenArr )
{
	// `NICK``yuandre``USER``yuandre``yuandre``localhost``:Görkem``Sever`
	std::cout << YELLOW << "NICK" << END << std::endl;
	if (tokenArr.size() < 2 || tokenArr[1][0] == '#') // saçma bir nick girme işlemi gerçekleşirse önlemek için. // kullanıcı adının başında # olamaz.
	{
		it->sendMessageFd(ERR_ERRONEUSNICKNAME(it->getPrefix(), tokenArr[1]));
		if (it->getRegistered() == false) // kayıt enasında hatalı bir kullanım söz konusuysa server'dan atmamız lazım.
		{
			tokenArr.clear();
			tokenArr.push_back("Nickname is wrong use.");
			Server::quit(it, tokenArr); // boş gönderirsem seg. yiyor
		}
	}
	else if (it->getRegistered() == false) //welcome mesajı için mecburi bu if olmalı, olmasada çalışıyor.
	{
		if (Server::findClientName(tokenArr[1]) != -1)
			it->sendMessageFd(ERR_NICKNAMEINUSE(tokenArr[1]));
		else
		{
			it->setNickname(tokenArr[1]);
			std::vector<std::string> cmd;
			cmd.push_back("CAP");
			cmd.push_back("END");
			Server::cap(it, cmd);
			cmd.clear();
		}
	}
	else
	{
		if (tokenArr[1].empty())
			it->sendMessageFd(ERR_NONICKNAMEGIVEN(tokenArr[1]));
		else if (Server::findClientName(tokenArr[1]) != -1)
			it->sendMessageFd(ERR_NICKNAMEINUSE(tokenArr[1]));
		else
		{
			it->sendMessageFd(RPL_NICK(it->getPrefix(), tokenArr[1]));
			it->setNickname(tokenArr[1]);
		}
	}
}

void	Server::pass( Client* it, std::vector<std::string> tokenArr ) // OK
{
	// `PASS``asdf``NICK``yuandre``USER``yuandre``yuandre``localhost``:Görkem``Sever`
	std::cout << YELLOW << "PASS" << END << std::endl;
	if (!tokenArr.empty() && !tokenArr[0].compare("PASS"))
		if (!tokenArr[1].compare(this->_password))
			it->setPasswordStatus();
		else
		{
			tokenArr.clear();
			tokenArr.push_back("Password is wrong.");
			Server::quit(it, tokenArr);
		}
	else
	{
		tokenArr.clear();
		tokenArr.push_back("Password is empty.");
		Server::quit(it, tokenArr);
	}
}

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

void	Server::infoFd( void )
{
	std::cout << BLUE << "------------POLL.FDS-----------------" << END << std::endl;
	for (size_t i = 0; i < this->_pollfds.size(); i++)
		std::cout << this->_pollfds[i].fd << " - ";
	std::cout << BLUE << "\n------------CLIENTS.FDS--------------" << END << std::endl;
	for (itClients it = _clients.begin(); it != _clients.end(); it++)
		std::cout << it->first << " - ";
	std::cout << BLUE << "\n------------CHANNELS.FDS-------------" << END << std::endl;
	for (itChannels it = _channels.begin(); it != _channels.end(); it++)
		std::cout << it->first << " - ";
	std::cout << BLUE << "\n-------------------------------------" << END << std::endl;
}

void	Server::removeClient(int clientFd)
{
	itClients it = _clients.find(clientFd);
	if (it != _clients.end())
	{
		delete it->second; // Belleği serbest bırak
		_clients.erase(it);
	}
}

//"USER akaraca akaraca localhost :Ahmet Karaca" komutu aşağıdaki anlamı taşır:
//Kullanıcı Adı (Username): "akaraca"
//Nick Adı (Nickname): "akaraca"
//Host Adı (Hostname): "localhost" (genellikle boş bırakılır)
//Gerçek İsim (Realname): "Ahmet Karaca"
// /set nick ...
// /set user_name ...
// /set hostname ...
// /set real_name ...
void Server::user(Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "USER" << END << std::endl;
	if (tokenArr.size() < 4) // saçma sapan bir set'leme gerçekleşirme önlemek için.
	{
		tokenArr.clear();
		tokenArr.push_back("User block is wrong.");
		Server::quit(it, tokenArr);
		return ;
	}
	if (it->getActiveStatus() == true) // quit attıktan sonra USER komutunu işlemeye çalışıyor lakin yarıda kesiliyor bu yüzden aktif mi değil mi kontrol etmek zorundayım.
	{
		it->setUsername(tokenArr[1]);
		std::cout << "Username:>" << it->getUsername() << std::endl;

		if (it->getNickname().empty()) // aynı isimde kullanıcı gelme durumunda, değişmesin istiyoruz, çünkü bu durumda nickname daha önce atanmış oluyor.
			it->setNickname(tokenArr[2]);
		std::cout << "Nickname:>" << it->getNickname() << std::endl;

		it->setHostname(tokenArr[3]);
		std::cout << "Hostname:>" << it->getHostname() << std::endl;

		tokenArr[4].erase(0, 1); 	// token[4] token4'un basindaki : kaldırır
		for (size_t i = 5; i < tokenArr.size(); i++)
			tokenArr[4].append(" " + tokenArr[i]); // realname boş olabilir veya birden fazla argümana sahip olabilir deneyin: /set real_name A B C D E F
		it->setRealname(tokenArr[4]);
		std::cout << "Realname:>" << it->getRealname() << std::endl;
		if (it->getPasswordStatus() == false)
			Server::pass(it, std::vector<std::string>());
	}
}

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

/**
 * @brief 
 * 
 * @link https://www.antionline.com/showthread.php?136933-IRC-flags
 * @link https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.5
 * @link https://irssi.org/documentation/help/mode/
 * 
 * PRIVMSG
Message:>MODE-naber<
Message:>naber<
Tokens:>`naber`<
MODE
Message:>MODE-#naber<
Message:>#naber<
Tokens:>`#naber`<
MODE
Message:>MODE-naber asdf<
Message:>naber asdf<
Tokens:>`naber``asdf`<
MODE
Message:>MODE-#naber +asdf<
Message:>#naber +asdf<
Tokens:>`#naber``+asdf`<
MODE
Message:>MODE-#naber +asdf<
Message:>#naber +asdf<
Tokens:>`#naber``+asdf`<
MODE
Message:>MODE-#naber -asdf<
Message:>#naber -asdf<
Tokens:>`#naber``-asdf`<
MODE
Message:>MODE-naber -asdf<
Message:>naber -asdf<
Tokens:>`naber``-asdf`<
MODE
Message:>MODE-naber -asdf asdf asdf<
Message:>naber -asdf asdf asdf<
Tokens:>`naber``-asdf``asdf``asdf`<
MODE
Message:>MODE-#naber -asdfasdfasdf<
Message:>#naber -asdfasdfasdf<
Tokens:>`#naber``-asdfasdfasdf`<
MODE
Message:>MODE-#naber +asdfasdfasdfasdfasfasdf<
Message:>#naber +asdfasdfasdfasdfasfasdf<
Tokens:>`#naber``+asdfasdfasdfasdfasfasdf`<
MODE
^C

 * @link https://matrix-org.github.io/matrix-appservice-irc/latest/irc_modes.html
 * 
 * @param message 
 */
void	Server::mode( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "MODE" << END << std::endl;
	std::cout << "tokenarr[0]: " << tokenArr[0] << std::endl;
	if (tokenArr.size() < 2)
	{
		it->sendMessageFd(ERR_NEEDMOREPARAMS(it->getNickname(), tokenArr[0]));
		return;
	}

	// #define RPL_MODE(source, channel, modes, args)	":" + source + " MODE " + channel + " " + modes + " " + args
	// it->sendMessageFd(RPL_MODE(it->getPrefix(), it->get));
}

void	Server::ping( Client* it, std::vector<std::string> tokenArr ) // OK
{
	if (!tokenArr[1].compare(it->getNickname())
			|| !tokenArr[1].compare(this->_host))
		it->sendMessageFd(ERR_NEEDMOREPARAMS(it->getNickname(), "PING"));
	it->sendMessageFd(RPL_PING(it->getNickname(), tokenArr[1]));
}

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

/*
	"/list -y" yazınca tüm kanalları listeleyecek.
 
	#42kocaeli
	#BEn
	#VoidChannel
	...
*/
void	Server::list( Client* it, std::vector<std::string> tokenArr )
{
	(void)tokenArr;
	// Serverdeki channelleri yazdiriyor.

	if (tokenArr[1].empty())
	{
		// Channel Users Name
		// 16:10 -!- End of /LIST
	}

	it->sendMessageFd(">-------- Server's 'Channel' list; --------<");
	for	(itChannels itChan = this->_channels.begin();\
			itChan != this->_channels.end(); itChan++)
	{
		it->sendMessageFd("LIST :" + itChan->first);
	}
	it->sendMessageFd("<------------- LIST END ------------------>");
}

/*

		311     RPL_WHOISUSER
						"<nick> <user> <host> * :<real name>"
		312     RPL_WHOISSERVER
						"<nick> <server> :<server info>"
		313     RPL_WHOISOPERATOR
						"<nick> :is an IRC operator"
		317     RPL_WHOISIDLE
						"<nick> <integer> :seconds idle"
		318     RPL_ENDOFWHOIS
						"<nick> :End of /WHOIS list"

	BUNLARIN HEPSİ BURADA GÖNDERİLMELİ
*/
void	Server::whois( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "WHOIS" << END << std::endl;
	int fd = Server::findClientName(tokenArr[1]);
	if (fd != -1)
	{
		Client *dest = _clients.at(fd);
		it->sendMessageFd(RPL_WHOISUSER(it->getNickname(), dest->getNickname(), dest->getUsername(), dest->getHostname(), dest->getRealname()));
		it->sendMessageFd(RPL_ENDOFWHOIS(it->getNickname(), dest->getNickname()));
	}
	else
		it->sendMessageFd(ERR_NOSUCHNICK(it->getNickname(), tokenArr[1])); // bu hata mesajı sonrasında whowas döndürüyor
}

void	Server::info( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "INFO" << END << std::endl;
	(void)it;
	(void)tokenArr;
	// Burada info komutu cagirildiginda Server'le ilgili bilgilerin,
	// 	server'e bagli olan userlerin -> realname'si yani Gorkem SEVER.
	// Server ne zaman kuruldu? Ne icin bu server kullanilir gibi bilgiler
	// 	yazan bir komut.
}

/*
	//Channel içindeyken /who yazınca bu çıktıyı veriyor, ana sayfaya.

	"/who asdf" yazarsan, asdf'nin başında # olmadığı içni ele alma.
	"/who #asdf" ise, asdf channelini ara ve kullancıların bilgilerini bastır.
	"/who #A #B #C" şeklindeki bir yapıda sadece #A ele alınacaktır.
	17:13 -!- #42kocaeli akaraca   H   0  
						~akaraca@2d6b-1b29-411f-5a9-8db6.88.176.ip 
						[k2m15s08.42kocaeli.com.tr]
	17:13 -!- #42kocaeli gsever    H@  4  
						~gsever@2d6b-1b29-411f-5a9-8db6.88.176.ip [Gorkem 
						Sever]
	17:13 -!- End of /WHO list

	--> ERR_NOSUCHCHANNEL
	--> RPL_WHOREPLY
	--> RPL_ENDOFWHO

*/
void	Server::who( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "WHO" << END << std::endl;
	if (tokenArr[1][0] == '#') //kanal aramak için
	{
		itChannels itC = _channels.find(tokenArr[1]); // channeli bul.
		if (itC != _channels.end()) // eğer channel varsa
		{
			Channel* itChanel = itC->second;
			for (size_t i = 0; i < itChanel->_channelClients.size(); i++)
			{
				Client* A = itChanel->_channelClients[i];
				std::string isAdmin = "";
				if (A == itChanel->getAdmin())
					isAdmin = "@";
				it->sendMessageFd(RPL_WHOREPLY(it->getPrefix(), tokenArr[1], A->getUsername(), A->getHostname(), A->getServername(), A->getNickname(), isAdmin,"0", A->getRealname()));
				// @ -> kullanıcının operator olduğunu temsil ediyor.
				// "0" ise kullanıcının o anki sunucu üzerinden mesajlaştığını ifade eder.

				//  * abc       H   3
				// "H 3" ifadesi, kullanıcının "abc" takma adına sahip olduğunu ve sunucu ile arasında 3 "hop" olduğunu belirtir. Yani, bu kullanıcı ile mesajlar üç aracı sunucu aracılığıyla iletilmiştir.
			}
			it->sendMessageFd(RPL_ENDOFWHO(it->getPrefix(), tokenArr[1]));
		}
		else // channel yoksa hata döndür.
			it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[1])); // #x: No such channel
	}
	else // Burada sadece serverda arama yaptığımız için admin işaretine gerek yok??
	{
		//	Server'da kullanıcıyı aramak için yapılıyor.
		//	Channel bilgisi "*" şeklinde gözüküyor.
		//	Aradağı kişi yoksa herhangi bir çıktı vermiyor sadece End of /WHO list çıktısı bulunuyor.
			// 09:56 -!-          * akaraca   H   3  ~akaraca@bbdb-4a67-88a1-bfa9-1d6d.190.78.ip [ahmet karaca]
			// 09:56 -!- End of /WHO list
		// "/who akaraca X Ben" şeklinde bir girdi olsada sadece akaraca hakkında bilgi veriliyor.
		int fd = Server::findClientName(tokenArr[1]);
		if (fd != -1)
		{
			Client *itC = _clients.at(fd);
			it->sendMessageFd(RPL_WHOREPLY(it->getPrefix(), "*", itC->getUsername(), itC->getHostname(), itC->getServername(), itC->getNickname(), "@","0", itC->getRealname()));
		}
		it->sendMessageFd(RPL_ENDOFWHO(it->getPrefix(), "*"));
	}
}

// **************************************
// IRC (Internet Relay Chat), internet üzerinden gerçek zamanlı metin tabanlı iletişim için bir protokoldür. IRC sunucuları, sunucunun ve kanallarının farklı yönlerini kontrol etmek ve yönetmek için çeşitli komut bayrakları kullanır. Bu komut bayrakları genellikle '/mode' komutuyla birlikte kullanılır ve bayrağın ayarlandığını veya ayarlanmadığını belirtmek için önünde bir artı ('+') veya eksi ('-') işareti bulunur.

// Aşağıda bazı yaygın IRC sunucusu komut bayraklarının bir listesi verilmiştir:

// - `+b` ve `-b`: Bu bayraklar, bir kullanıcı veya ana makine maskesine yasak koymak veya kaldırmak, böylece onların bir kanala katılmasını engellemek için kullanılır.
// - `+e` ve `-e`: Bu bayraklar, bir yasağın istisnasını ayarlamak veya kaldırmak için kullanılır ve bir kullanıcının veya ana bilgisayar maskesinin yasaklanmış olsa bile bir kanala katılmasına olanak tanır.
// - `+I` ve `-I`: Bu bayraklar, bir kanalın davet listesine bir kullanıcıyı veya ana bilgisayar maskesini eklemek veya kaldırmak için kullanılır; bu, yalnızca davetle olarak ayarlanmış olsa bile bu kişilerin kanala katılmasına olanak tanır.
// - `+k` ve `-k`: Bu bayraklar, kullanıcıların kanala katılabilmeleri için sağlamaları gereken kanal anahtarını (şifre) ayarlamak veya kaldırmak için kullanılır.
// - `+l` ve `-l`: Bu bayraklar bir kanalda kullanıcı limiti ayarlamak veya kaldırmak için kullanılır ve kanala katılabilecek maksimum kullanıcı sayısını kontrol eder.
// - `+m` ve `-m`: Bu bayraklar bir kanalda denetlenen modu ayarlamak veya kaldırmak için kullanılır; bu, yalnızca ses veya operatör ayrıcalıklarına sahip kullanıcıların kanala mesaj gönderebileceği anlamına gelir.
// - `+n` ve `-n`: Bu bayraklar bir kanalda harici mesaj yok modunu ayarlamak veya kaldırmak için kullanılır; bu, kanala yalnızca kanalda bulunan kullanıcıların mesaj gönderebileceği anlamına gelir.
// - `+o` ve `-o`: Bu bayraklar, bir kanaldaki kullanıcıya operatör ayrıcalıkları vermek veya bu ayrıcalıkları almak için kullanılır.
// - `+p` ve `-p`: Bu bayraklar bir kanalda özel modu ayarlamak veya kaldırmak için kullanılır; bu, kanalın sunucunun kanal listesinde gösterilmeyeceği anlamına gelir.
// - `+s` ve `-s`: Bu bayraklar bir kanalda gizli modu ayarlamak veya kaldırmak için kullanılır; bu, kanalın sunucunun kanal listesinde gösterilmeyeceği ve kullanıcıların kimlerin kanalda olduğunu göremeyeceği anlamına gelir. onlar da içinde olmadığı sürece kanal.
// - `+t` ve `-t`: Bu bayraklar bir kanalda konu kilitleme modunu ayarlamak veya kaldırmak için kullanılır; bu, yalnızca operatör ayrıcalıklarına sahip kullanıcıların kanalın konusunu değiştirebileceği anlamına gelir.