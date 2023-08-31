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
 * izlemek için(Mac'te): nettop -m tcp	
 * 
 * IRC url:	https://www.youtube.com/watch?v=JpLwB14L2Rk&ab_channel=TallPaulTech
 * 			https://www.youtube.com/watch?v=njKv66miR80&list=PL5tDlMcZytRqvYbDWLoayxAkUcHsOxX_p&index=9
 * 
 * 			https://beej.us/guide/bgnet/html/split/
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
 mode, part, privmsg 
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
