#ifndef	CLIENT_HPP
# define CLIENT_HPP

# include <vector>
# include <string>
# include <iostream>

class Client
{
	private:
		int				_fd;
		unsigned short	_port;
		std::string		_hostname;
		std::string		_nickname;
		std::string		_username;
		std::string		_realname;
		std::string		_servername;
		bool			_isRegistered;

	public:
		Client( int fd, int port, const std::string &hostname );
		~Client();

		int				getFd( void ) { return (this->_fd); }
		unsigned short	getPort( void ) { return (this->_port); }
		std::string		getHostname( void ) { return (this->_hostname); }
		std::string		getNickname( void ) { return (this->_nickname); }
		std::string		getUsername( void ) { return (this->_username); }
		std::string		getRealname( void ) { return (this->_realname); }
		std::string		getServername( void ) { return (this->_servername); }
		bool			getRegistered( void ) { return (this->_isRegistered); }

		void	setHostname( std::string name ) { this->_hostname = name; }
		void	setNickname( std::string name ) { this->_nickname = name; }
		void	setUsername( std::string name ) { this->_username = name; }
		void	setRealname( std::string name ) { this->_realname = name; }
		void	setServername( std::string name ) { this->_servername = name; }
		void	setRegistered( void ) { this->_isRegistered = true; }
};

#endif