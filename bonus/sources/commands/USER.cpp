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

	bool reNick = false;
	if (it->getNickname().empty() && Server::getClientFdByNickname(tokenArr[2]) != -1) // eğer nicki yoksa ve aynı username'ye sahip biri varsa, USER'dan nicke gönderilecek ve nick'te welcome mesajı ile karşılanacak.
	{
		reNick = true;
		it->setIRCstatus(RENICK);
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), tokenArr[2], "Client status is now: RENICK."));
		it->sendMessageFd(ERR_NICKNAMEINUSE(tokenArr[2])); // Client, server'a kayıtlı aynı nick'e sahip bir kullanıcı varsa, bu ismi alamayacağını belirtmek için vardır.
	}
	if (!it->getNickname().empty() && Server::getClientFdByNickname(it->getNickname()) != it->getFd()) //Eğerki nicki boş gelmiyorsa, ve kendi fd'sine denk değilse yani kendinden başkası bu nicki kullanmıyorsa tekrardan nick istenir.
	{
		reNick = true;
		it->setIRCstatus(RENICK);
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client status is now: RENICK."));
		it->sendMessageFd(ERR_NICKNAMEINUSE(it->getNickname())); // Client, server'a kayıtlı aynı nick'e sahip bir kullanıcı varsa, bu ismi alamayacağını belirtmek için vardır.
	}

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
	
	if (reNick == false) // değilse burada welcome mesajı yazdırılacak.
	{
		it->setIRCstatus(AUTHENTICATED);
		it->sendWelcomeMessage(Server::welcomeServer()); // ilk bağlantı olduğundan dolayı, emoji mesajıdır
		it->sendMessageFd(RPL_WELCOME(it->getNickname(), _serverName));  // ilk bağlantı olduğundan dolayı, selamlama mesajıdır
		it->sendMessageFd(RPL_NICK(it->getPrefix(), it->getNickname())); // Client'e kullanıcı adının değiştiği bilgisi verilir.
		std::stringstream ss;
		ss << it->getPort();
		it->sendMessageFd(RPL_NOTICE(it->getPrefix() + ":" + ss.str() , it->getNickname(), "Client status is now: AUTHENTICATED."));
	}
}

//END -> NICK -> USER
//END -> PASS -> NICK -> USER -> NICK

//END -> USER => NİCK KONTROL -> EĞER VARSA HATA DÖNDÜR -> NICK
