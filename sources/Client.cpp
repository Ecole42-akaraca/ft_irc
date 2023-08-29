#include "Client.hpp"

Client::Client( int fd, int port, const std::string &hostname )
		: _fd(fd),
		_port(port),
		_hostname(hostname),
		_isRegistered(false),
		_isPasswordOK(false),
		_isActive(true)
{
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

std::string	Client::getPrefix() const
{
	return this->_nickname
		+ (this->_username.empty() ? "" : "!" + this->_username)
		+ (this->_hostname.empty() ? "" : "@" + this->_hostname);
}

void	Client::sendMessageFd( std::string message )
{
	std::cout << YELLOW << "Response:>" + message << "<" << END << std::endl;
	message += "\r\n";
	if (send(this->getFd(), message.c_str(), message.size(), 0) == -1)
		throw (std::runtime_error(\
"Error: sendMessageFd: Failed to send message."));
}
