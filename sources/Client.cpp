#include "Client.hpp"

Client::Client( int fd, int port, const std::string &hostname )
		: _clientFd(fd),
		_port(port),
		_hostName(hostname),
		_state(HANDSHAKE) // _channel(NULL)
{ (void)_clientFd; (void)_port; (void)_state; }

Client::~Client() {}
