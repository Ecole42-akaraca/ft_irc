#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <exception>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>

# include "../colors.hpp"
# include "../Channel.hpp"

# include <unistd.h>

class Channel;

class Bot
{
/* _________________________ VARIABLES ______________________________________ */
	private:
		bool							_isCheck;
		const std::string				_host; // 127.0.0.1
		const unsigned short			_port; // 8888
		const std::string				_password; // asdf
		bool							_isRun;
		int								_botFd;
		std::map<std::string, Channel*>	_channels;
		std::string						_botNickname;
		std::string						_botUsername;
		std::string						_botRealname;
/* -------------------------------------------------------------------------- */
/* _________________________ MAIN FUCTIONS __________________________________ */
	private:
		Bot( void );
	public:
		Bot( int argc, char **argv );
		~Bot();
		void	start( void );
/* -------------------------------------------------------------------------- */
/* _________________________ COMMANDS _______________________________________ */
		void	authenticate( void );
		void	sendMessageToServer( std::string message );
/* -------------------------------------------------------------------------- */
/* _________________________ SOCKET FUCTIONS ________________________________ */
	private:
		void			openSocket( void );
		void			connectSocketAddress( void );
/* -------------------------------------------------------------------------- */
/* _________________________ UTILS __________________________________________ */
	private:
		bool								check( int argc );
		unsigned short						port( std::string argv );
		std::string							password( std::string argv );
		std::string							welcomeBot( void );
/* -------------------------------------------------------------------------- */	
};

#endif
