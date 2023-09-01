#include "../../includes/Server.hpp"

/*
	"/list -y" yazınca tüm kanalları listeleyecek.
 
	#42kocaeli
	#BEn
	#VoidChannel
	...
*/
void	Server::list( Client* it, std::vector<std::string> tokenArr )
{
	(void)tokenArr;
	// Serverdeki channelleri yazdiriyor.

	if (tokenArr[1].empty())
	{
		// Channel Users Name
		// 16:10 -!- End of /LIST
	}

	it->sendMessageFd(">-------- Server's 'Channel' list; --------<");
	for	(itChannels itChan = this->_channels.begin();\
			itChan != this->_channels.end(); itChan++)
	{
		it->sendMessageFd("LIST :" + itChan->first);
	}
	it->sendMessageFd("<------------- LIST END ------------------>");
}