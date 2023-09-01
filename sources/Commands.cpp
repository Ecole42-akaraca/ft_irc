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

void	Server::removeClient(int clientFd)
{
	itClients it = _clients.find(clientFd);
	if (it != _clients.end())
	{
		delete it->second; // Belleği serbest bırak
		_clients.erase(it);
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