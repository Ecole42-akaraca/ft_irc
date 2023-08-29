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