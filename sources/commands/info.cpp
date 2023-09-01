#include "../../includes/Server.hpp"

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