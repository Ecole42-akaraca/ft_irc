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
		std::string				_name;
		int						_clientCount;
		Client*		_admin;
		// std::vector<Client*>	_admins; // Birden fazla admin.
		std::string				_channelTopic; // Channel'e bağlantı kurulduğunda konu başlığının belirlenmesinde yardımcı oluyor.

		std::string	_k; // Channel şifresini belirliyor
		int			_l; // Channel'a bağlanacak max client sayısını temsil ediyor.

	public:
		Channel( std::string name, std::string password, Client* admin );
		~Channel( void );
		std::vector<Client *>	_channelClients;

		std::string	getName( void ) { return (this->_name); }
		Client*		getAdmin( void ) { return (this->_admin); }
		// std::vector<Client*>	getAdmins( void ) { return (this->_admins); }
		std::string getChannelTopic( void ) { return (this->_channelTopic); }
		int			getClientCount( void ) { return (this->_clientCount); }
		std::string	getPassword( void ) { return (this->_k); }
		int			getMaxClient( void ) { return (this->_l); }

		void		setName( std::string &name ) { this->_name = name; }
		void		setChannelTopic( std::string topic ) { this->_channelTopic = topic; }
		void		setAdmin( Client* admin ) { this->_admin = admin; }
		// void		setAdmins( std::vector<Client*> admins ) { this->_admins = admins; }
		void		setPassword( std::string password ) {this->_k = password; }
		void		setMaxClient( int maxClient ) {this->_l = maxClient; }

		void		addClient( Client* client );
		// void		addAdmin( Client* admin ) { this->_admins.push_back(admin); }

		void		removeClient( Client* client );

		void		sendMessageBroadcast( std::string message );
		void		sendMessageBroadcast( Client* exclude, std::string message );
		void		channelUsers(Client* client, Channel* channel, std::string channelName ); // Channel'e girişte kullanıcıların listesini channel'e bastırıyor.
};

#endif
