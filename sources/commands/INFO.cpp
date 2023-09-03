# include "../../includes/Server.hpp"

void	Server::info( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "INFO" << END << std::endl;
	(void)tokenArr;
	it->sendMessageFd(RPL_INFO(it->getPrefix(), std::string("Bu ft_irc projesi, akaraca ve gsever tarafından oluşturulmuştur.")));
	it->sendMessageFd(RPL_INFO(it->getPrefix(), std::string("Serverdaki kullanıcıların ifşaları:")));
	it->sendMessageFd(RPL_ENDOFINFO(it->getPrefix()));
	// Burada info komutu cagirildiginda Server'le ilgili bilgilerin,
	// 	server'e bagli olan userlerin -> realname'si yani Gorkem SEVER.
	// Server ne zaman kuruldu? Ne icin bu server kullanilir gibi bilgiler
	// 	yazan bir komut.
}
