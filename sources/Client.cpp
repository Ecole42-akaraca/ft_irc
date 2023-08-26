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

Client	&Client::operator=( Client &rhs )
{
	this->_fd = rhs.getFd();
	this->_port = rhs.getPort();
	this->_hostname = rhs.getHostname();
	this->_nickname = rhs.getNickname();
	this->_username = rhs.getUsername();
	this->_realname = rhs.getRealname();
	this->_servername = rhs.getServername();
	this->_isRegistered = rhs.getRegistered();
	return (*this);
}

Client::~Client() {}
	