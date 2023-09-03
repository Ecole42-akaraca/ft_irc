# include "../../includes/Server.hpp"

void	Server::nick( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "NICK" << END << std::endl;
	if (tokenArr.size() < 2 || tokenArr[1][0] == '#')
	{
		std::string name = "";
		if (tokenArr.size() > 1)
			name = tokenArr[1];
		it->sendMessageFd(ERR_ERRONEUSNICKNAME(it->getPrefix(), name));
		if (it->getIRCstatus() != AUTHENTICATED)
			Server::quitReason(it, "Nickname is wrong use!");
		return ;
	}

	if (tokenArr[1].empty())
		it->sendMessageFd(ERR_NONICKNAMEGIVEN(tokenArr[1]));
	else if (Server::getClientFdByNickname(tokenArr[1]) != -1)
		it->sendMessageFd(ERR_NICKNAMEINUSE(tokenArr[1]));
	else
	{
		if (it->getIRCstatus() == AUTHENTICATED)
			it->sendMessageFd(RPL_NICK(it->getPrefix(), tokenArr[1]));
		else
		{
			it->sendWelcomeMessage(Server::welcomeServer());
			it->sendMessageFd(RPL_WELCOME(tokenArr[1]));
			if (it->getPasswordStatus() == true) // re_nick durumunda yaşanan problemi atlamak için koydum.
				it->setIRCstatus(AUTHENTICATED);
		}
		it->setNickname(tokenArr[1]);
	}

}
