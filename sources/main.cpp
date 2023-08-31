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
 * TODO: 11:53 -!- Irssi: (default) critical nicklist_set_host: assertion 'host != NULL' failed
 * TODO: LEAK kontrolu gerekiyor.
 * TODO: mode, part, quit, privmsg, info, kick
 * TODO: macos'ta kayitli olmayan bir kullanici cikis yaparsa server patliyor. nc localhost 8888
 * TODO: /msg #asdf hello -> diye kanala mesaj gonderdigimizde okay, ama
 * 	/msg #gsever naber -> diye kullaniciya gonderdigimizde patliyor cunku
 * 	kullanicilarin isimleri # ile baslayamaz.
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
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
