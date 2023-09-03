#ifndef	CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include "Client.hpp"
# include "utils.hpp"
# include <algorithm>

class Client;

typedef	std::vector<Client *>::iterator itChannelClients;

class Channel
{
	private:
		std::string	_name;
		int			_clientCount;
		Client*		_admin;
		std::string _channelTopic;

		std::string	_k;

	public:
		Channel( std::string name, std::string password, Client* admin );
		~Channel( void );
		std::vector<Client *> _channelClients;

		std::string	getName( void ) { return (this->_name); }
		Client*		getAdmin( void ) { return (this->_admin); }
		std::string getChannelTopic( void ) { return (this->_channelTopic); }
		int			getClientCount( void ) { return (this->_clientCount); }
		// std::string	getPassword( void ) { return (this->_k); }

		void		setName( std::string &name ) { this->_name = name; }
		void		setChannelTopic( std::string topic ) { this->_channelTopic = topic; }
		void		setAdmin( Client* admin ) { this->_admin = admin; }
		// void		setPassword( std::string password ) {this->_k = password; }

		// bool		ifClientJoined( Client* client );

		void		addClient( Client* client );
		void		removeClient( Client* client );

		void		sendMessageBroadcast( std::string message );
		void		sendMessageBroadcast( Client* exclude, std::string message );
		void		channelUsers(Client* client, Channel* channel, std::string channelName );
};

#endif
