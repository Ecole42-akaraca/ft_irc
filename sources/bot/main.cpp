#include "Bot.hpp"

int	main( int argc, char **argv )
{
	(void)argc;
	try
	{
		Bot	ircBot(argc, argv);
		std::cout << "Bot created succesfully!" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}