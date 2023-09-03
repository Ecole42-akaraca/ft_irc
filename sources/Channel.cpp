# include "../includes/Channel.hpp"

Channel::Channel( std::string name, std::string password, Client* admin )
	: _name(name), _clientCount(0), _admin(admin), _k(password)
{
	std::cout << "Channel Created: Name: " << this->getName() << std::endl;
}

Channel::~Channel( void )
{
	std::cout << "Channel Deleting: Name: " << this->getName() << std::endl;
	delete [] (this);
}

void	Channel::addClient( Client* client )
{
	this->_channelClients.push_back(client);
	this->_clientCount++;
	std::cout << this->getName() << ": " << client->getNickname()
		<< " joined." << std::endl;
}

void	Channel::removeClient( Client* client )
{
	itChannelClients it = std::find(_channelClients.begin(), _channelClients.end(), client);
	if (it != _channelClients.end())
	{
		_channelClients.erase(it);
		this->_clientCount--;
		std::cout << this->getName() << ": " << client->getNickname()
			<< " removed." << std::endl;
	}
}

void	Channel::sendMessageBroadcast( std::string message )
{
	for	(itChannelClients itCli = this->_channelClients.begin();
			itCli != this->_channelClients.end(); itCli++)
	{
		(*itCli)->sendMessageFd(message);
	}
}

/**
 * @brief Eger bir Client Channel'e mesaj attiginda burasi calisacak.
 * 
 * Cunku yazdigi mesaji kanaldaki herkese gonderip, kendisine gondermemesi lazim.
 * 
 * @param exclude Mesaji gonderen Client'in kendisi.
 * @param message 
 */
void	Channel::sendMessageBroadcast( Client* exclude, std::string message )
{
	for	(itChannelClients itCli = this->_channelClients.begin(); itCli != this->_channelClients.end(); itCli++)
	{
		if (*itCli == exclude)
			continue;
		(*itCli)->sendMessageFd(message);
	}
}

void	Channel::channelUsers(Client* client, Channel* channel, std::string channelName )
{
	for (size_t i = 0; i <  channel->_channelClients.size(); ++i)
	{
		std::string authority = "";
		if (channel->_channelClients[i]->getNickname().compare(channel->getAdmin()->getNickname()) == 0)
			authority = "@";
		client->sendMessageFd(RPL_NAMREPLY(client->getNickname(), channelName, authority + channel->_channelClients[i]->getNickname()));
	}
	client->sendMessageFd(RPL_ENDOFNAMES(client->getPrefix(), channelName));
}
