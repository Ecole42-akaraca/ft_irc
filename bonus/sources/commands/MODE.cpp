# include "../../includes/Server.hpp"

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
 * 
 * @param message 
 * 
 * 
 * 
 * 
 * MODE message:
 * 		->User mode: Bu modlar, kullanıcıların kendilerinin veya diğer kullanıcıların nasıl görüneceğini veya hangi ekstra mesajları alacaklarını değiştirmek için kullanılır.
 * 			'/mode <otherone_nick> -i' -> 'Can't change mode for other users' hata alırsın çünkü başka birinin mode ayarlarını değiştiremezsin. Yetki yeterli değildir.
 * 			'/mode <own_nick> -i' -> 'Mode change [-i] for user akaraca2' kendi mode ayarlarını değiştirebilirsin.
 * 
 * Mode change [+i] for user akaraca2
 * Mode change [+H] for user akaraca2
 * Mode change [+s] for user akaraca2
 * Mode change [+w] for user akaraca2
 * 	-> Bu yapıların '-'li durumları kullanımına izin var(dalnet)
 * 
 * 		->Channel mode
 * 			'/mode #42kocaeli +o <other_nick>' -> kişiye adminlik veriliyor. Bizim yapıda 1 adet adminlik olduğundan dolayı, admin takası gerçekleşecek bu durumda.
 * 			'/mode #42kocaeli +k test' -> şeklinde channel'e şifre koyulursa:
 * 				sadece '/join 42kocaeli' girdisi sonucunda şu hatayı alacaktır -> 'Cannot join to channel #42kocaeli (Bad channel key)'
 * 				girebilmesi için : '/join 42kocaeli test' yazmlıdır.
 * 			'/mode #42kocaeli +l 3' -> Channel'daki kullanıcı sınırını belirliyor
 * 				eğer 4.kişi girmeye çalışırsa -> 'Cannot join to channel #42kocaeli (Channel is full)' hatası ile karşılaşıyor 4.client
 * 			'/mode #42kocaeli -l' ile limit kaldırılıyor.
 * 
 */
void	Server::mode( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "MODE" << END << std::endl;
	if (it->getIRCstatus() != AUTHENTICATED)
	{
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
		return ;
	}

	if (tokenArr.size() < 3)
	{
		it->sendMessageFd(ERR_NEEDMOREPARAMS(it->getPrefix(), tokenArr[0]));
		return ;
	}

	if (tokenArr[1][0] != '#')
	{
		it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[1]));
		return ;
	}

	itChannels itChan = _channels.find(tokenArr[1]);
	if (itChan == _channels.end())
	{
		it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[1]));
		return ;
	}

	if (!isChannelAdmin(it, itChan->second))
	{
		it->sendMessageFd(ERR_CHANOPRIVSNEEDED(it->getPrefix(), tokenArr[1]));
		return ;
	}

	for (size_t i = 0; i < tokenArr[2].size(); ++i)
	{
		char c = tokenArr[2][i];
		char prevC = i > 0 ? tokenArr[2][i - 1] : '\0';
		bool active = prevC == '+';
		switch (c)
		{
			case 'k':
			{
				itChan->second->setPassword( active ? tokenArr[3] : "" );
				itChan->second->sendMessageBroadcast(RPL_MODE(it->getPrefix(), tokenArr[1], (active ? "+k" : "-k"), (active ? tokenArr[3] : "")));
				break;
			}

			case 'l':
			{
				itChan->second->setMaxClient(active ? atoi(tokenArr[3].c_str()) : -1);
				itChan->second->sendMessageBroadcast(RPL_MODE(it->getPrefix(), tokenArr[1], (active ? "+l" : "-l"), (active ? tokenArr[3] : "")));
				break;
			}

			case 'o':
			{
				Client* user = getClientByNickname(tokenArr[3]);
				if (user != NULL)
				{
					if (isChannelUser(user, itChan->second))
					{
						itChan->second->setAdmin(active ? user : NULL);
						itChan->second->sendMessageBroadcast(RPL_MODE(it->getPrefix(), tokenArr[1], (active ? "+o" : "-o"), (active ? tokenArr[3] : "")));
					}
					else
						it->sendMessageFd(ERR_USERNOTINCHANNEL(it->getPrefix(), tokenArr[3], tokenArr[1]));
				}
				else
					it->sendMessageFd(ERR_NOSUCHNICK(it->getPrefix(), tokenArr[3]));
				break;
			}
		
			default:
				break;
		}
	}

	//Sadece Channel mode işlemini ele alıyoruz.
	//if (tokenArr.size() > 2)
	//{
	//	if (tokenArr[1][0] == '#') // Argümanın channel tipinde olup olmadığı kontrol ediliyor.
	//	{
	//		itChannels itChan = _channels.find(tokenArr[1]);
	//		if (itChan != _channels.end()) // Eğer channel varsa
	//		{
	//			if (isChannelAdmin(it, itChan->second)) // Mode kullanan admin mi kontrol ediliyor.
	//			{
	//				Client* user = getClientByNickname(tokenArr[3]); // Mode'un uygulanacak kişisi var mı kontrol ediliyor.
	//				if (user != NULL)
	//				{
	//					if (isChannelUser(user, itChan->second)) // Uygulanacak kişi aynı channel'da mı?
	//					{
	//						// Mode komutlarının kontrolü burada yapılıyor...
	//						if (tokenArr[2][0] == '+')
	//						{
	//							if (tokenArr[2].find("o"))  //adminlik yetkisi veriliyor mu?
	//							{
	//								itChan->second->setAdmin(user);
	//								itChan->second->sendMessageBroadcast(RPL_MODE(it->getPrefix(), tokenArr[1], "+o", user->getNickname()));
	//								itChan->second->sendMessageBroadcast(RPL_MODE(it->getPrefix(), tokenArr[1], "-o", it->getNickname()));
	//							}
	//							if (tokenArr[2].find("k")) // Channel password belirleniyor
	//							{
	//								itChan->second->setPassword(tokenArr[3]);
	//								itChan->second->sendMessageBroadcast(RPL_MODE(it->getPrefix(), tokenArr[1], "+k", user->getNickname()));
	//							}
	//							if (tokenArr[2].find("l")) // Channel'e bağlanıcak max kullanıcı sayısı
	//							{
	//								itChan->second->setMaxClient(atoi(tokenArr[3].c_str()));
	//							}
	//						}
	//						else if (tokenArr[2][0] == '-')
	//						{
	//							if (tokenArr[2].find("o"))
	//							{
	//								itChan->second->setAdmin(NULL);
	//								itChan->second->sendMessageBroadcast(RPL_MODE(it->getPrefix(), tokenArr[1], "-o", it->getNickname()));
	//							}
	//							if (tokenArr[2].find("k"))
	//							{
	//								itChan->second->setPassword("");
	//								itChan->second->sendMessageBroadcast(RPL_MODE(it->getPrefix(), tokenArr[1], "-k", user->getNickname()));
	//							}
	//							if (tokenArr[2].find("l"))
	//							{
	//								itChan->second->setMaxClient(-1);
	//							}
	//						}
	//						else
	//							it->sendMessageFd(ERR_UNKNOWNCOMMAND(it->getPrefix(), tokenArr[2]));
	//					}
	//					else
	//						it->sendMessageFd(ERR_USERNOTINCHANNEL(it->getPrefix(), tokenArr[3], tokenArr[1]));
	//				}
	//				else
	//					it->sendMessageFd(ERR_NOSUCHNICK(it->getPrefix(), tokenArr[3]));
	//			}
	//			else
	//				it->sendMessageFd(ERR_CHANOPRIVSNEEDED(it->getPrefix(), tokenArr[1]));
	//		}
	//		else // Eğer channel yoksa
	//			it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[1]));
	//	}
	//}
	//else
	//	it->sendMessageFd(ERR_NEEDMOREPARAMS(it->getPrefix(), tokenArr[0]));
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
