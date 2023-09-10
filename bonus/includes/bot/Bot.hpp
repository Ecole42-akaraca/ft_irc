#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <string>
# include <exception>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
# include <cstring>
# include <limits>
# include <pthread.h>
# include <unistd.h>
# include <map>
# include <algorithm>

# include "../colors.hpp"
# include "../Channel.hpp"
# include "../Server.hpp"

# define MAX_BUFFER 1024

// enum e_badWords
// {
	
// };

class Channel;

// typedef std::map<std::string, Channel *>::iterator	itChannels;
// typedef std::map<std::string, std::string>::iterator	itBadWords;

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
	public:
/* -------------------------------------------------------------------------- */
/* _________________________ MAIN FUCTIONS __________________________________ */
	private:
		Bot( void );
	public:
		Bot( int argc, char **argv );
		~Bot();
		static std::map<std::string, std::string>	_dataBadWords; // Static function'dan ulasabilmemiz icin bu variable'yi de static tanimlamamiz gerekiyor.
		void			start( void );
		static void*	listen( void * ); // POSIX thread's recv() function.
/* -------------------------------------------------------------------------- */
/* _________________________ COMMANDS _______________________________________ */
		void	authenticate( void );
		void	sendMessageToServer( std::string message );
		void	checkChannels( void );
		void	joinChannels( void );
		static std::vector<std::string>\
			tokenMessage( std::string message );
		static void	onMessageReceive( std::string buffer );
		static std::map<std::string, std::string>\
			botSplitMessage( std::string delimeter, std::string message );
		static void			initBadWords( void );
		// static std::string	filterBadWords( std::string& text );
		static std::string	scanNickname( std::string message );
		static std::string	toLowerCase( const std::string& str );
/* -------------------------------------------------------------------------- */
/* _________________________ SIGNAL FUCTIONS ________________________________ */
		static void	sigHandler( int signalNum );
/* -------------------------------------------------------------------------- */
/* _________________________ SOCKET FUCTIONS ________________________________ */
	private:
		void			openSocket( void );
		void			connectSocketAddress( void );
/* -------------------------------------------------------------------------- */
/* _________________________ UTILS __________________________________________ */
	private:
		bool			check( int argc );
		unsigned short	port( std::string argv );
		std::string		password( std::string argv );
		std::string		welcomeBot( void );
/* -------------------------------------------------------------------------- */	
};

#endif
