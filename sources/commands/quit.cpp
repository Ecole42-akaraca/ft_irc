# include "../../includes/Server.hpp"

void	Server::quit( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "QUIT" << END << std::endl;
	it->setIRCstatus(DISCONNECTED);

	for (size_t i = 0; i < it->getRegisteredChannels().size(); ++i)
	{
		std::cout << __LINE__ << std::endl;
		std::vector<std::string> leaveChannel;
		leaveChannel.push_back("PART");
		leaveChannel.push_back(it->getRegisteredChannels()[i]->getName());
		leaveChannel.push_back(":QUIT");
		Server::part(it, leaveChannel);
		if (i != it->getRegisteredChannels().size())
			i--;
	}
	it->clearRegisteredChannels();

	Server::serverInfo();
	for (size_t i = 0; i < _pollfds.size(); i++)
	{
		if (it->getFd() == _pollfds[i].fd)
		{
			it->sendMessageFd(RPL_QUIT(it->getPrefix(), tokenArr[0]));
			close(it->getFd());
			Server::removeClient(it->getFd());
			close(_pollfds[i].fd);
			_pollfds.erase(_pollfds.begin() + i);
			break;
		}
	}
	Server::serverInfo();
}
