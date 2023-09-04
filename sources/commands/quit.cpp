# include "../../includes/Server.hpp"

/*
	// :syrk!kalt@millennium.stealth.net QUIT :Gone to have lunch
*/
void	Server::quit( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "QUIT" << END << std::endl;
	it->setIRCstatus(DISCONNECTED);
	it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client status is now: DISCONNECTED."));

	for (size_t i = 0; i < it->getRegisteredChannels().size(); ++i) // Quit ile ayrılan channel, kayıtlı olduğu channel'lardan ayrılmalıdır.
	{
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
