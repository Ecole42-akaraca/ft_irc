#ifndef	SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstdlib>
# include <stdint.h>
# include <limits>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <cstring>
# include <netinet/in.h>
# include <unistd.h>
# include <fcntl.h>
# include <map>
# include <vector>
# include <sys/poll.h>
# include <netdb.h>
# include <sstream> 
# include "Client.hpp"
# include "Channel.hpp"
# include "utils.hpp"
# include "Colors.hpp"

# define MAX_CONNECTIONS 10
# define MAX_BUFFER 1024

class Client;
class Channel;
class Server;

typedef std::vector<pollfd>::iterator				itPoll;
typedef std::map<int, Client *>::iterator			itClients;
typedef std::map<std::string, Channel *>::iterator	itChannels;
typedef std::vector<std::string>::iterator			itCmd;
typedef std::vector<std::string>::iterator			itMessage;

// Commands
// typedef void (Server::*CommandFunction)( Client*, std::string );
typedef void (Server::*CommandFunction)( Client*, std::vector<std::string> );
typedef std::map<std::string, CommandFunction>::iterator	itCommandFunction;

class Server
{
	private:
		bool					_isCheck; // arguments size is okay.
		const unsigned short	_port; // server port number.
		const std::string		_password; // server password.
		const std::string		_host; // server ip 127.0.0.1
		bool					_isRun; // program is running?
		int						_serverFd; // server's fd number.
		std::vector<pollfd>		_pollfds;
		std::map<int, Client*>	_clients; // client's with fd numbers.
		std::map<std::string, Channel *>	_channels; // channel's vector.
		// std::vector<std::string>			_inputToken; // client'ten gelen mesajin tokenlerine ayrilmis hali.

		Server( void ); // Default Constructor.
		void			openSocket( void );
		void			setSocketOptions( void );
		void			createSocketAddress( void );
		void			startListening( void );
		std::string		welcomeServer( void );

	public:
		Server( int argc, char **argv );
		~Server( void ); // Destructor.
/* _________________________ MAIN FUCTION ___________________________________ */
		void	start( void );
		void	acceptClients( void );
		void	commandHandler( itPoll &itClient );
/* -------------------------------------------------------------------------- */
/* _________________________ COMMANDS _______________________________________ */
	private:
		std::map<std::string, CommandFunction> t_cmdFunc;
		void	cap( Client*, std::vector<std::string> );
		void	join( Client*, std::vector<std::string> );
		void	nick( Client*, std::vector<std::string> );
		void	pass( Client*, std::vector<std::string> );
		void	quit( Client*, std::vector<std::string> );
		void	privmsg( Client*, std::vector<std::string> );
		void	user( Client*, std::vector<std::string> );
		// void mode( Client*, std::vector<std::string> );
		void	ping( Client*, std::vector<std::string> );
		void	part( Client*, std::vector<std::string> );
		void	list( Client*, std::vector<std::string> );
		void	info( Client*, std::vector<std::string> );
		void	whois( Client*, std::vector<std::string> );
		void	infoFd( void );
		void	removeClient( int clientFd );
/* -------------------------------------------------------------------------- */
/* _________________________ UTILS __________________________________________ */
		bool			check( int argc );
		unsigned short	port( std::string argv );
		std::string		password( std::string argv );
		void			addToPollfds( int fd,  short events, short revents );
		void			initCommands( void );
		std::map<std::string, std::string> splitMessage( std::string message );
		std::vector<std::string>	cmdMessage( std::string message );
		std::string		trim(const std::string& str);
		int				findClientName( std::string name );
/* -------------------------------------------------------------------------- */
};

#endif