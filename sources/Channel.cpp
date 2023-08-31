#include "Channel.hpp"

Channel::Channel( std::string name, std::string password, Client* admin )
	: _name(name), _clientCount(1), _admin(admin), _k(password)
{
	std::cout << "Channel Created: Name: " << this->getName() << std::endl;
}

Channel::~Channel( void )
{
	std::cout << "Channel Deleting: Name: " << this->getName() << std::endl;
	delete [] (this);
}

// Client&	Channel::getAdmin( Client* it, std::string adminName )
// {
// 	std::string	admin;
// 	for (size_t i = 0; i < this->_admin.size(); i++)
// 	{
// 		if (adminName.compare(this->_admin[i]))
// 		{
// 			admin = this->_admin[i];
// 			break;
// 		}
// 	}
// 	for (size_t i = 0; i < this->_channelClients.size(); i++) // butun client'leri tariyoruz.
// 	{
// 		if (adminName.compare(this->_channelClients[i]->getNickname())) // her client'in nickname'siyle adminName'sini karsilastiriyoruz.
// 		{
// 			if (!this->_admin[i].compare(adminName))
// 				it->sendMessageFd(ERR_CHANOPRIVSNEEDED(it->getPrefix(), this->getName()));
// 			else
// 				return (*this->_channelClients[i]);
// 			break ;
// 		}
// 	}
// 	it->sendMessageFd(RED "This is not ADMIN!" END);
// }

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
	_channelClients.erase(it);
	this->_clientCount--;
	std::cout << this->getName() << ": " << client->getNickname()
		<< " removed." << std::endl;
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