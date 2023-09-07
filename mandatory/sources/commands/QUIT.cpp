# include "../../includes/Server.hpp"

/*
	// :syrk!kalt@millennium.stealth.net QUIT :Gone to have lunch
*/
void	Server::quit( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "QUIT" << END << std::endl;
	Server::serverInfo();
	Server::leaveAllChannel(it);
	it->setIRCstatus(DISCONNECTED);
	std::cout << "Connection closed by client " << it->getHostname() << ":" << it->getPort() << std::endl;
	it->sendMessageFd(RPL_QUIT(it->getPrefix(), tokenArr.at(0)));
}
