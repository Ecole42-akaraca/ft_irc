#include "Channel.hpp"

Channel::Channel( std::string name, std::string password, Client *admin )
	: _name(name), _clientCount(1), _admin(admin), _k(password)
{
	std::cout << "Channel Created: Name: " << this->getName() << std::endl;
}

Channel::~Channel( void )
{
	std::cout << "Channel Deleting: Name: " << this->getName() << std::endl;
	delete [] (this);
}

/**
 * @brief Eger bu Client Channel'in icerisinde var mi?
 * 
 * Channel'in icerisindeki Client'lerin hepsine bakiyor,
 * 	buldugunda 1 bulamadiginda 0 donduruyor.
 * 
 * @param client 
 * @return true	:Found.
 * @return false :NOT Found.
 */
bool	Channel::ifClientJoined( Client* client )
{
	std::string	clientNickName;

	clientNickName = client->getNickname();
	for (size_t i = 0; i < this->_channelClients.size(); i++)
	{
		if (clientNickName.compare(this->_channelClients[i]->getNickname()))
			return (1);
	}
	return (0);
}

void	Channel::addClient( Client client )
{
	this->_channelClients.push_back(&client);
	std::cout << this->getName() << ": " << client.getNickname()
		<< " joined." << std::endl;
}

void	Channel::sendMessageBroadcast( std::string message )
{
	for	(itChannelClients itCli = this->_channelClients.begin();
			itCli != this->_channelClients.end(); itCli++)
	{
		(*itCli)->sendMessageFd(message);
	}
}