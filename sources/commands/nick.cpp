#include "../../includes/Server.hpp"

void	Server::nick( Client* it, std::vector<std::string> tokenArr )
{
	// `NICK``yuandre``USER``yuandre``yuandre``localhost``:Görkem``Sever`
	std::cout << YELLOW << "NICK" << END << std::endl;
	if (tokenArr.size() < 2 || tokenArr[1][0] == '#') // saçma bir nick girme işlemi gerçekleşirse önlemek için. // kullanıcı adının başında # olamaz.
	{
		it->sendMessageFd(ERR_ERRONEUSNICKNAME(it->getPrefix(), tokenArr[1]));
		if (it->getRegistered() == false) // kayıt enasında hatalı bir kullanım söz konusuysa server'dan atmamız lazım.
		{
			tokenArr.clear();
			tokenArr.push_back("Nickname is wrong use.");
			Server::quit(it, tokenArr); // boş gönderirsem seg. yiyor
		}
	}
	else if (it->getRegistered() == false) //welcome mesajı için mecburi bu if olmalı, olmasada çalışıyor.
	{
		if (Server::findClientName(tokenArr[1]) != -1)
			it->sendMessageFd(ERR_NICKNAMEINUSE(tokenArr[1]));
		else
		{
			it->setNickname(tokenArr[1]);
			std::vector<std::string> cmd;
			cmd.push_back("CAP");
			cmd.push_back("END");
			Server::cap(it, cmd);
			cmd.clear();
		}
	}
	else
	{
		if (tokenArr[1].empty())
			it->sendMessageFd(ERR_NONICKNAMEGIVEN(tokenArr[1]));
		else if (Server::findClientName(tokenArr[1]) != -1)
			it->sendMessageFd(ERR_NICKNAMEINUSE(tokenArr[1]));
		else
		{
			it->sendMessageFd(RPL_NICK(it->getPrefix(), tokenArr[1]));
			it->setNickname(tokenArr[1]);
		}
	}
}
