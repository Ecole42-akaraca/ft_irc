# include "../../includes/Server.hpp"

void Server::user(Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "USER" << END << std::endl;

	if (it->getIRCstatus() == HANDSHAKE)
	{
		if (tokenArr.size() < 5)
		{
			Server::quitReason(it, "Not enough parameters for USER command.");
			return ;
		}
		if (!it->getNickname().empty()) // aynı nickli bir kullanıcı girmediği durumda statü değiştirmek için.
			it->setIRCstatus(AUTHENTICATED);

		it->setUsername(tokenArr[1]);
		std::cout << "Username:>" << it->getUsername() << std::endl;

		if (it->getNickname().empty())
			it->setNickname(tokenArr[2]);
		std::cout << "Nickname:>" << it->getNickname() << std::endl;

		it->setHostname(tokenArr[3]);
		std::cout << "Hostname:>" << it->getHostname() << std::endl;

		tokenArr[4].erase(0, 1);
		for (size_t i = 5; i < tokenArr.size(); i++)
			tokenArr[4].append(" " + tokenArr[i]);
		it->setRealname(tokenArr[4]);
		std::cout << "Realname:>" << it->getRealname() << std::endl;
		if (it->getPasswordStatus() == false)
			Server::pass(it, std::vector<std::string>());
	}
}
