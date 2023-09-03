# include "../../includes/Server.hpp"

void	Server::part( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "PART" << END << std::endl;
	if (it->isRegisteredChannel(tokenArr[1]) == true)
	{
		Channel *chan = _channels[tokenArr[1]];
		std::string msg = "";
		if (tokenArr.size() > 2)
		{
			tokenArr[2].erase(0, 1);
			msg = Server::combineMessage(2, tokenArr);
		}
		it->sendMessageFd(RPL_PART(it->getPrefix(), tokenArr[1], msg));
		chan->sendMessageBroadcast(it, RPL_PART(it->getPrefix(), tokenArr[1], msg));
		chan->removeClient(it);
		it->unregisterChannel(chan);
		if (chan->getClientCount() == 0)
			Server::removeChannel(chan->getName());
		else if (Server::isChannelAdmin(it, chan) == true)
		 	chan->setAdmin(chan->_channelClients[0]);
	}
	else
		it->sendMessageFd(ERR_NOSUCHCHANNEL(it->getPrefix(), tokenArr[1]));
}
