#include "../includes/Client.hpp"

Client::Client( int fd, int port, const std::string &hostname )
		: _fd(fd),
		_port(port),
		_hostname(hostname),
		_havePassword(false),
		_ircstatus(CONNECTING)
{
	this->_nickname = "";
	this->_username = "";
	this->_realname = "";
}

Client::~Client() {}

std::string	Client::getPrefix() const
{
	return this->_nickname
		+ (this->_username.empty() ? "" : "!~" + this->_username)
		+ (this->_hostname.empty() ? "" : "@" + this->_hostname);
}

void	Client::sendMessageFd( std::string message )
{
	std::cout << YELLOW << "Response:>" + message << "<" << END << std::endl;
	message += "\r\n";
	if (send(this->getFd(), message.c_str(), message.size(), 0) == -1)
		std::cerr << "Error: sendMessageFd: Failed to send message. Error code: " << errno << std::endl;
}

void	Client::sendWelcomeMessage( std::string message )
{
	message += "\r\n";
	if (send(this->getFd(), message.c_str(), message.size(), 0) == -1)
		std::cerr << "Error: sendMessageFd: Failed to send message. Error code: " << errno << std::endl;
}

void	Client::registerChannel( Channel* channel )
{
	this->_registeredChannels.push_back(channel);
}

void	Client::unregisterChannel( Channel* channel )
{
	for (size_t i = 0; i < this->_registeredChannels.size(); ++i)
	{
		if (this->_registeredChannels[i]->getName().compare(channel->getName()) == 0)
			this->_registeredChannels.erase(this->_registeredChannels.begin() + i);
	}
}

bool	Client::isRegisteredChannel( std::string channelName )
{
	for (size_t i = 0; i < this->_registeredChannels.size(); ++i)
	{
		std::string name = this->_registeredChannels[i]->getName();
		if (name.compare(channelName) == 0 && name.size() == channelName.size())
			return (true);
	}
	return (false);
}
