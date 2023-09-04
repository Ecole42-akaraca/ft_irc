#include "Bot.hpp"

/**
 * @brief Construct a new Bot:: Bot object
 * 
 * ./ircbot 127.0.0.1 8888 asdf
 * 
 * @param argc 
 * @param argv 
 */
Bot::Bot( int argc, char **argv )
	: _host(argv[2]), _port(std::atoi(argv[3])), _password(argv[4])
{ (void)argc; }

Bot::~Bot( void )
{
}