# include "../../includes/Server.hpp"

/*
//"USER akaraca akaraca localhost :Ahmet Karaca" komutu aşağıdaki anlamı taşır:
//Kullanıcı Adı (Username): "akaraca"
//Nick Adı (Nickname): "akaraca"
//Host Adı (Hostname): "localhost" (genellikle boş bırakılır)
//Gerçek İsim (Realname): "Ahmet Karaca"
// /set nick ...
// /set user_name ...
// /set hostname ...
// /set real_name ...
*/
void Server::user(Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "USER" << END << std::endl;

	if (it->getIRCstatus() != HANDSHAKE) // User komutu sadece tanışma kısmında kullanılmalıdır
	{
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
		return ;	
	}

	if (tokenArr.size() < 5) // bağlanan kullanıcının yeterli USER verisi yoksa server'dan atılır.
	{
		Server::quitReason(it, "Not enough parameters for USER command.");
		return ;
	}

	if (it->getPasswordStatus() == false)
	{
		Server::pass(it, std::vector<std::string>());
		return;
	}

	it->setIRCstatus(RENICK);
	std::vector<std::string> arr;
	arr.push_back("NICK");
	if (!it->getNickname().empty())
		arr.push_back(it->getNickname());
	Server::nick(it, arr);

	it->setUsername(tokenArr[1]);
	std::cout << "Username:>" << it->getUsername() << std::endl;

	if (it->getNickname().empty()) // Eğerki nickname boş gelirse, nickname'i username ile aynı yapacağız.
		it->setNickname(tokenArr[2]); // User'a gelen bu token'de username'yi temsil etmektedir. Nickname değildir.
	std::cout << "Nickname:>" << it->getNickname() << std::endl;

	it->setHostname(tokenArr[3]);
	std::cout << "Hostname:>" << it->getHostname() << std::endl;

	tokenArr[4].erase(0, 1); // token[4] token4'un basindaki : kaldırır
	for (size_t i = 5; i < tokenArr.size(); i++)
		tokenArr[4].append(" " + tokenArr[i]); // realname boş olabilir veya birden fazla argümana sahip olabilir deneyin: /set real_name A B C D E F
	it->setRealname(tokenArr[4]);
	std::cout << "Realname:>" << it->getRealname() << std::endl;

}

//END -> NICK -> USER
//END -> PASS -> NICK -> USER -> NICK

//END -> USER => NİCK KONTROL -> EĞER VARSA HATA DÖNDÜR -> NICK
