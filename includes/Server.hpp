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

# define MAX_CONNECTIONS 10
# define MAX_BUFFER 1024

class Client;

class Channel;

typedef std::vector<pollfd>::iterator itPoll;
typedef std::map<int, Client *>::iterator itClients;
typedef std::map<std::string, Channel *>::iterator itChannels;

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
/* _________________________ COMMANDS __________________________________________ */
	private:
		typedef void (Server::*CommandFunction)( Client*, std::string );
		std::map<std::string, CommandFunction> t_cmdFunc;
		void cap( Client*, std::string message );
		void join( Client*, std::string message );
		void nick( Client*, std::string message );
		void privmsg( Client*, std::string message );
		void user( Client*, std::string message );
/* -------------------------------------------------------------------------- */
/* _________________________ UTILS __________________________________________ */
		bool			check( int argc );
		unsigned short	port( std::string argv );
		std::string		password( std::string argv );
		void			addToPollfds( int fd,  short events, short revents );
		void			initCommands( void );
		std::map<std::string, std::string> splitMessage( std::string message );
		std::string		trim(const std::string& str);
/* -------------------------------------------------------------------------- */
};

#endif