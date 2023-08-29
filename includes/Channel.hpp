#ifndef	CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include "Client.hpp"
// # include "Server.hpp"

class Client;

typedef	std::vector<Client *>::iterator itChannelClients;

class Channel
{
	private:
		std::string	_name;
		int			_clientCount;
		Client		*_admin;

		std::string	_k;

	public:
		Channel( std::string name, std::string password,\
			Client *admin );
		~Channel( void );
		std::vector<Client *> _channelClients;

		std::string	getName( void ) { return (this->_name); }
		int			getClientCount( void ) { return (this->_clientCount); }
		std::string	getPassword( void ) { return (this->_k); }
		Client		&getAdmin( void ) { return (*this->_admin); }

		void		setName( std::string &name ) { this->_name = name; }
		void		setClientCount( int count ) { this->_clientCount = count; }
		void		setAdmin( Client &client ) { *this->_admin = client; }
		void		setPassword( std::string password ) {this->_k = password; }

		bool		ifClientJoined( Client* client );

		void		addClient( Client client );

		void		sendMessageBroadcast( std::string message );
};

#endif