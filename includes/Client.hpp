#ifndef	CLIENT_HPP
# define CLIENT_HPP

# include <vector>
# include <string>
# include <iostream>
# include <sys/socket.h>
# include <errno.h>

# include "colors.hpp"
# include "Channel.hpp"

class Channel;

enum IRCstatus{
	CONNECTING, // Bu durum, IRC client'ın IRC server'a bağlanmaya çalıştığı durumu temsil eder.
	HANDSHAKE, // Bu durum, bağlantının kurulduğu ve IRC client'in IRC server ile bir kimlik oluşturmaya çalıştığı durumu temsil eder.
	AUTHENTICATED, // Bu durum, IRC client'in IRC server tarafından kimlik doğrulaması yapıldığı durumu temsil eder.
	DISCONNECTED, // Bu durum, bağlantının sonlandırıldığı durumu temsil eder.
};

class Client
{
	private:
		int						_fd;
		unsigned short			_port;
		std::string				_hostname;
		std::string				_nickname;
		std::string				_username;
		std::string				_realname;
		bool					_havePassword;
		int						_ircstatus;
		std::vector<Channel*>	_registeredChannels;

	public:
		Client( int fd, int port, const std::string &hostname );
		~Client();

		int				getIRCstatus( void ) { return (this->_ircstatus); }
		std::string		getPrefix() const;
		int				getFd( void ) { return (this->_fd); }
		unsigned short	getPort( void ) { return (this->_port); }
		std::string		getHostname( void ) { return (this->_hostname); }
		std::string		getNickname( void ) { return (this->_nickname); }
		std::string		getUsername( void ) { return (this->_username); }
		std::string		getRealname( void ) { return (this->_realname); }
		bool			getPasswordStatus( void ) { return (this->_havePassword); }

		void	setIRCstatus( int status ) { this->_ircstatus = status; }
		void	setHostname( std::string name ) { this->_hostname = name; }
		void	setNickname( std::string name ) { this->_nickname = name; }
		void	setUsername( std::string name ) { this->_username = name; }
		void	setRealname( std::string name ) { this->_realname = name; }
		void	setPasswordStatus( void ) { this->_havePassword = true; }
		void	sendMessageFd( std::string message );
		void	sendWelcomeMessage( std::string message );

		void	registerChannel( Channel* channel );
		void	unregisterChannel( Channel* channel );
		std::vector<Channel*> getRegisteredChannels( void ) { return (this->_registeredChannels); }
		bool	isRegisteredChannel( std::string channelName );
		void	clearRegisteredChannels( void ) { this->_registeredChannels.clear(); }
};

#endif

/*
Waiting for CAP LS response...
03:02 -!- Irssi: Connection to irc.dal.net established
03:02 !atw.hu.eu.dal.net *** Looking up your hostname...
03:02 !atw.hu.eu.dal.net *** Checking Ident
03:02 !atw.hu.eu.dal.net *** Found your hostname
03:02 !atw.hu.eu.dal.net *** No Ident response
03:02 -!- Welcome to the DALnet IRC Network akaraca!~akaraca@78.190.134.3
03:02 -!- Your host is atw.hu.eu.dal.net, running version bahamut-2.2.2
03:02 -!- This server was created Sat 08 Oct CEST at 2022 02:30:58 PM
03:02 -!- atw.hu.eu.dal.net bahamut-2.2.2 aAbcCdefFghHiIjkKmnoOPrRsSwxXy AbceiIjklLmMnoOpPrRsStv beIjklov
03:02 -!- NETWORK=DALnet SAFELIST MAXBANS=200 MAXCHANNELS=50 CHANNELLEN=32 KICKLEN=307 NICKLEN=30 TOPICLEN=307 MODES=6 CHANTYPES=#
          CHANLIMIT=#:50 PREFIX=(ohv)@%+ STATUSMSG=@%+ are available on this server
03:02 -!- CASEMAPPING=ascii WATCH=128 SILENCE=10 ELIST=cmntu EXCEPTS INVEX CHANMODES=beI,k,jl,ciPAmMnOprRsSt MAXLIST=b:200,e:100,I:100
          TARGMAX=DCCALLOW:,JOIN:,KICK:4,KILL:20,NOTICE:20,PART:,PRIVMSG:20,WHOIS:,WHOWAS: are available on this server
03:02 -!- There are 81 users and 6904 invisible on 38 servers
03:02 -!- 74 IRC Operators online
03:02 -!- 3755 channels formed
03:02 -!- I have 1727 clients and 1 servers
03:02 -!- Current local users: 1727 Max: 2113
03:02 -!- Current global users: 6985 Max: 12060
03:02 !atw.hu.eu.dal.net *** Notice -- motd was last changed at 20/1/2021 6:02
03:02 -!- - atw.hu.eu.dal.net Message of the Day -
03:02 -!- -20/1/2021 6:02
03:02 -!- - Welcome to DALnet!  By connecting to this network, you agree to abide
03:02 -!- - by its acceptable use policy, available here: http://www.dal.net/aup
03:02 -!- -
03:02 -!- - Server Policies:
03:02 -!- -
03:02 -!- -    *   We reserve the right to deny access to this server without
03:02 -!- -        warning or explanation
03:02 -!- -    *   No malicious bots, including flood and nick/channel chasers
03:02 -!- -    *   No channel filler bots/clients/psybncs
03:02 -!- -          - A channel filler is a client, bnc, or bot which is
03:02 -!- -            connected just to fill a channel and make it look
03:02 -!- -            bigger
03:02 -!- -    *   Operator impersonation, spamming, or any other abuse will
03:02 -!- -        cause you to be banned from this server
03:02 -!- -    *   Abide by DALnet's AUP (www.dal.net/aup)
03:02 -!- -    *   Please have fun and enjoy DALnet!
03:02 -!- -
03:02 -!- -      ///admins////////////////////////////////////////
03:02 -!- -      //                                             //
03:02 -!- -      //  [ Hidden  - administrator             ]    //
03:02 -!- -      //                                             //
03:02 -!- -      //  [ koszik  - assistant administrator   ]    //
03:02 -!- -      //                                             //
03:02 -!- -      ///operators/////////////////////////////////////
03:02 -!- -      //                                             //
03:02 -!- -      //                                             //
03:02 -!- -      //                                             //
03:02 -!- -      /////////////////////////////////////////////////
03:02 -!- -
03:02 -!- -
03:02 -!- - For IRCop assistance, please /join #operhelp or message one of our
03:02 -!- - friendly server staff listed above.
03:02 -!- -
03:02 -!- - For multilingual information:  <http://docs.dal.net/>
03:02 -!- -
03:02 -!- - Thank you for using DALnet!  We hope you have a pleasant day.
03:02 -!- -
03:02 -!- End of /MOTD command.
03:02 -!- Mode change [+iH] for user akaraca
*/