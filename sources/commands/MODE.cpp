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
 */
// void	Server::mode( Client* it, std::vector<std::string> tokenArr )
// {
// 	std::cout << YELLOW << "MODE" << END << std::endl;
// 	std::cout << "tokenarr[0]: " << tokenArr[0] << std::endl;
// 	if (tokenArr.size() < 2)
// 	{
// 		it->sendMessageFd(ERR_NEEDMOREPARAMS(it->getNickname(), tokenArr[0]));
// 		return;
// 	}

// 	// #define RPL_MODE(source, channel, modes, args)	":" + source + " MODE " + channel + " " + modes + " " + args
// 	// it->sendMessageFd(RPL_MODE(it->getPrefix(), it->get));
// }

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
