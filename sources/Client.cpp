#include "Client.hpp"

Client::Client( int fd, int port, const std::string &hostname )
		: _fd(fd),
		_port(port),
		_hostname(hostname),
		_firstContact()
{ (void)_fd; (void)_port; }

Client::~Client() {}
	