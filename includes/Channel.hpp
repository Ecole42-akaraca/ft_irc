#ifndef	CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"

class Client;

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

		void		addClient( Client client );
};

#endif