/**
 * @file DCC.cpp
 * @author Görkem SEVER (gsever), Ahmet KARACA(akaraca)
 * @brief 
 * @version 0.1
 * @date 2023-09-03
 * 
 * @link: https://github.com/irssi/irssi/tree/master/src/irc/dcc
 * @note: DCC (Direct Client-to-Client) mean; Send file with IRC.
 * 
 * @link: https://en.wikipedia.org/wiki/Reserved_IP_addresses
 * @link: http://www.iana.org/numbers
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../../includes/Server.hpp"

/**
 * @brief 
 * 
 * @note;
 * IRC (Internet Relay Chat) DCC (Direct Client-to-Client) komutları,
	kullanıcıların birbirleriyle doğrudan bağlantı kurmasını sağlar.

1.	**DCC CHAT {nick}**: Bu komut, belirtilen takma adla bir sohbet oturumu başlatır.
Örneğin, "DCC CHAT john" komutu, "john" adlı kullanıcıyla bir sohbet oturumu başlatır.
Bu hizmet, kullanıcıların bir DCC bağlantısı üzerinden birbirleriyle sohbet etmelerini sağlar.
Trafik doğrudan kullanıcılar arasında gider ve IRC ağı üzerinden gitmez.
DCC CHAT genellikle bir CTCP (Client-To-Client Protocol) HANSHAKE(el sıkışması) kullanılarak başlatılır.

2.	**DCC SEND {nick} {dosya}**: Bu komut, belirtilen takma ada belirtilen dosyayı gönderir.
Örneğin, "DCC SEND john image.jpg" komutu, "john" adlı kullanıcıya "image.jpg" dosyasını gönderir.

3.	**DCC GET {nick} {dosya}**: Bu komut, belirtilen takma addan belirtilen dosyayı alır.
Örneğin, "DCC GET john image.jpg" komutu, "john" adlı kullanıcıdan "image.jpg" dosyasını alır.

4.	**DCC RESUME {nick} {dosya} {port} {position}**: Bu komut, belirtilen takma adla belirtilen 
 dosyanın belirtilen konumdan indirilmesini sürdürür.
Örneğin, "DCC RESUME john image.jpg 5000 1024" komutu, "john" adlı kullanıcıdan "image.jpg"
 dosyasının 1024. bayttan itibaren indirilmesini sürdürür.

5.	**DCC ACCEPT {nick} {dosya} {port} {position}**: Bu komut, belirtilen takma adla belirtilen
 dosyanın belirtilen konumdan indirilmesini kabul eder.
Örneğin, "DCC ACCEPT john image.jpg 5000 1024" komutu, "john" adlı kullanıcıdan "image.jpg"
 dosyasının 1024. bayttan itibaren indirilmesini kabul eder.

Bu komutlar genellikle IRC istemcilerinde bulunur ve doğrudan IRC sunucusu yerine
 kullanıcılar arasında bağlantı kurarlar.
 * 
 */


/*
DCC (Direct Client-to-Client) protokolü, IRC (Internet Relay Chat) istemcileri arasında doğrudan bağlantılar kurmak için kullanılır¹. DCC bağlantıları, IRC ağı ve sunucularını atlayarak istemciler arasında çeşitli verilerin aktarılmasına olanak sağlar². İşte DCC protokolünün bazı temel komutları ve nasıl çalıştıkları:

1. **DCC CHAT**: Bu hizmet, kullanıcıların bir DCC bağlantısı üzerinden birbirleriyle sohbet etmelerini sağlar¹. Trafik doğrudan kullanıcılar arasında gider ve IRC ağı üzerinden gitmez¹. DCC CHAT genellikle bir CTCP (Client-To-Client Protocol) el sıkışması kullanılarak başlatılır¹.

2. **DCC SEND**: Bu hizmet, bir dosyanın bir kullanıcıdan diğerine gönderilmesini sağlar¹. Dosya transferi genellikle bir CTCP el sıkışması kullanılarak başlatılır¹.

3. **DCC RESUME**: Bu komut, durdurulmuş bir dosya transferini sürdürmek için kullanılır. Örnek: `/DCC RESUME KullaniciAdi dosya.zip`

4. **DCC ACCEPT**: Bu komut, durdurulmuş bir dosya transferinin devam etmesini kabul etmek için kullanılır. Örnek: `/DCC ACCEPT KullaniciAdi dosya.zip`

5. **DCC CLOSE**: Bu komut, bir DCC bağlantısını kapatmak için kullanılır. Örnek: `/DCC CLOSE get KullaniciAdi`

Bu komutların her biri, belirli bir DCC işlemi gerçekleştirmek için kullanılır. Her komutun tam olarak nasıl çalıştığını anlamak için, belirli bir IRC istemcisinin belgelerine veya yardım dosyalarına başvurmanız önerilir.

Umarım bu bilgiler yardımcı olur! Başka bir sorunuz varsa, lütfen sormaktan çekinmeyin. 😊

Source: Conversation with Bing, 03.09.2023
(1) Direct Client-to-Client - Wikipedia. https://en.wikipedia.org/wiki/Direct_Client-to-Client.
(2) Direct Client-to-Client Protocol (DCC). https://modern.ircdocs.horse/dcc.html.
(3) List of Internet Relay Chat commands - Wikipedia. https://en.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands.
*/