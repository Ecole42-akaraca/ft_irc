# include "../../includes/Server.hpp"

void	Server::privmsg( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "PRIVMSG" << END << std::endl;
	if (it->getIRCstatus() != AUTHENTICATED)
		return ;
	tokenArr[2].erase(0, 1);
	std::string msg = Server::combineMessage(2, tokenArr);
	if (tokenArr[1][0] == '#')
	{
		if (it->isRegisteredChannel(tokenArr[1]) == true)
			this->_channels[tokenArr[1]]->sendMessageBroadcast(it, RPL_PRIVMSG(it->getPrefix(), tokenArr[1], msg));
		else
			it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[1]));
	}
	else
	{
		int fd = Server::getClientFdByNickname(tokenArr[1]);
		if (fd != -1)
		{
			Client *destClient = _clients.find(fd)->second;
			destClient->sendMessageFd(RPL_PRIVMSG(it->getPrefix(), tokenArr[1], msg));
		}
		else
			it->sendMessageFd(ERR_NOSUCHNICK(it->getPrefix(), tokenArr[1]));
	}
}
