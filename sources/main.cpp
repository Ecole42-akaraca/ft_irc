#include "../includes/Server.hpp"

/**
 * @brief 
 * 
 * @TODO: Server olusturulduktan sonra client'ten gelen baglanti aninda;
 * Server; baglanan client'in 
 * 'file descriptor',
 * 'address' ve 
 * 'port'unu printle. Bunlari --DEBUG=true flagiyle calistirirken printlesin.
 * ornek: ./ircserv --DEBUG=true <port> <password>
 * istemci ornek:	telnet localhost 8080
 * 					nc localhost  8080
 * 					url olarak: localhost:8080
 * 
 * nc veya telnet ile argüman giriş sırası:
 * CAP END
 * PASS 1
 * NICK AHMET
 * USER AHMET akaraca localhost :Ahmet Karaca
 * 
 * izlemek için(Mac'te): nettop -m tcp	
 * 
 * IRC url:	https://www.youtube.com/watch?v=JpLwB14L2Rk&ab_channel=TallPaulTech
 * 			https://www.youtube.com/watch?v=njKv66miR80&list=PL5tDlMcZytRqvYbDWLoayxAkUcHsOxX_p&index=9
 * 
 * 			https://beej.us/guide/bgnet/html/split/
 * 
 * https://docs.racket-lang.org/irc/index.html#%28def._%28%28lib._irc%2Fmain..rkt%29._irc-set-nick%29%29
 * https://www.irchelp.org/protocol/rfc/chapter6.html
 * https://modern.ircdocs.horse/#rplliststart-321
 * https://www.rfc-editor.org/rfc/rfc2812#section-3.2.6
 * https://ircv3.net/specs/extensions/capability-negotiation.html
 * https://dd.ircdocs.horse/refs/commands/part
 * https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.5
 * https://www.alien.net.au/irc/irc2numerics.html
 * 
 * TODO: MODE; komutundaki parse'lenen inputa gore ayarla.
 * TODO: 11:53 -!- Irssi: (default) critical nicklist_set_host: assertion 'host != NULL' failed
 * TODO: NP: /nick ile ismimizi degistirdigimizde ana sayfada degil de channel'deyse channel'in icine
 *  ciktisini yazmasi lazim.
 * TODO: LEAK kontrolu gerekiyor.
 * TODO: Ilk baglantida ayni isimde nick varsa USER komutunda 
 * USER
	Username:>yuandre
	Nickname:>yuandre
	Hostname:>localhost
	Realname:>:Görkem Sever -> buradaki kisma girmiyor orasi duzeltilecek.
 * TODO: /msg ile olan durumu ele al; client'ten client'e ozel mesaj atma olayi.
 mode, part, privmsg info
 * TODO: Kayitli olmayan bir kullanici cikis yaparsa sever patliyor. nc localhost 8888
 * TODO: /msg #asdf hello -> diye kanala mesaj gonderdigimizde okay, ama
 * 	/msg #gsever naber -> diye kullaniciya gonderdigimizde patliyor cunku
 * 	kullanicilarin isimleri # ile baslayamaz.
 * TODO: PING var PONG'unu da ekle aynisi zaten kopyala yapistir pingi pong yap.
 * TODO: nc localhost ->dc problemi
 * TODO: Kişi channelde değilse ve channel varsa who komutunda patlıyor(/who channelname)
 * TODO: Not aliyorum; Mode, list, who, whois 
 * 		/ ":" kaldirma olaylari vardi, utilsden silmek daha mantikli 
 * 		/ main bloğunu düzenle, kaynak adreslerini ekle 
 * 		/ tüm test yapilarinin bulunduğu bir txt dosyasi hazirla 
 * 		/ mode komutunu diğer fonksiyonlara entegre et 
 * 		/ éger gerekirse utils sirasini düzelt 
 * 		/ bol bol test et.
 * 		/ github'daki yapılarıda ekle
 * @param argc: Server port number.
 * @param argv: Server password.
 * @return int 
 */
int	main( int argc, char **argv )
{

	try
	{
		Server Server(argc, argv);
		Server.start();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_SUCCESS);
	}
}
