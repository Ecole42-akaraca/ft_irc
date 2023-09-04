#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <exception>

class Bot
{
	private:
		const std::string		_host; // 127.0.0.1
		const unsigned short	_port; // 8888
		const std::string		_password; // asdf
		Bot( void );
	public:
		Bot( int argc, char **argv );
		~Bot();
};

#endif