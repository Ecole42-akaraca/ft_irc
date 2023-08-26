#include "Client.hpp"

Client::Client( int fd, int port, const std::string &hostname )
		: _fd(fd),
		_port(port),
		_hostname(hostname),
		_isRegistered(false)
{
	(void)_fd;
	(void)_port;
	(void)hostname;
	this->_nickname = "";
	this->_username = "";
	this->_realname = "";
	this->_servername = "";
}

Client::~Client() {}
	