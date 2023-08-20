#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <cstdlib>
# include "Colors.hpp"

class Utils{

	public:
		static void validateArguments(int argc, char **argv);
		static bool isValidPort(const std::string &port);

/* _________________________ EXCEPTIONS _____________________________________ */
	class exceptionInvalidArgument : public std::exception
	{
		private:
			std::string _message;
		public:
			exceptionInvalidArgument( const std::string &message ) : _message(message) {}
			~exceptionInvalidArgument() throw() {}
			const char *what() const throw(){
				return (_message.c_str());
			}
	};
};

#endif
