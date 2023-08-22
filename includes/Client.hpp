#ifndef	CLIENT_HPP
# define CLIENT_HPP

#include <vector>
#include <string>

enum ClientState {
	HANDSHAKE,
	LOGIN,
	PLAY,
	DISCONNECTED
};

class Client
{
	private:
		int				_clientFd;
		unsigned short	_port;
		std::string		_hostName;
		std::string		_nickName;
		std::string		_userName;
		std::string		_realName;
		ClientState		_state;

	public:
		Client( int fd, int port, const std::string &hostname );
		~Client();
};

#endif