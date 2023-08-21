/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaraca <akaraca@student.42.tr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:26:25 by gsever            #+#    #+#             */
/*   Updated: 2023/08/20 18:10:44 by akaraca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstdlib>
# include <stdint.h>
# include <limits>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <cstring>
# include <netinet/in.h>
# include <unistd.h>
# include <fcntl.h>
# include "Colors.hpp"

# define MAX_CONNECTIONS 1000
# define MAX_BUFFER 1024

class Server
{
	private:
		bool					_check;
		const unsigned short	_port;
		const std::string		_password;
		const std::string		_host;
		int						_serverSocket;
		int						_clientSocket; // _activeSocket;
		struct sockaddr_in 		_serverAddr;
		struct sockaddr_in 		_clientAddr;
		char					_buffer[MAX_BUFFER];
		bool					_running;
		Server( void ); // Default Constructor.
	public:
		Server( int argc, char **argv );
		~Server( void ); // Destructor.
/* _________________________ MAIN FUCTION ___________________________________ */
		void	start( void );
		void	newSocket( void );
/* -------------------------------------------------------------------------- */
/* _________________________ SET/GET FUNCTIONS ______________________________ */
/* -------------------------------------------------------------------------- */
/* _________________________ TIME FUNCTIONS _________________________________ */
/* -------------------------------------------------------------------------- */
/* _________________________ IF FUNCTIONS ___________________________________ */
/* -------------------------------------------------------------------------- */
/* _________________________ PRINT FUCTIONS _________________________________ */
/* -------------------------------------------------------------------------- */
/* _________________________ UTILS __________________________________________ */
	private:
		bool			check( int argc );
		unsigned short	port( std::string argv );
		std::string		password( std::string argv );
/* -------------------------------------------------------------------------- */
/* _________________________ EXCEPTIONS _____________________________________ */

	class exceptionInvalidArgument : public std::exception
	{
		private:
			std::string _message;
		public:
			exceptionInvalidArgument( const std::string &message ) : _message(message) {}
			~exceptionInvalidArgument() throw() {}
			const char *what() const throw(){
				return ( _message.c_str() );
			}
	};

/* exception -> Array Not Sorted */

	// class exceptionArrayNotSorted : public std::exception
	// {
	// 	public:
	// 		virtual const char	*what() const throw();
	// };
/* ------------------------- */
/* exception -> Invalid Argument */

	// class exceptionInvalidArgument : public std::exception
	// {
	// 	private:
	// 		std::string	_message;
	// 	public:
	// 		exceptionInvalidArgument( void );
	// 		exceptionInvalidArgument( std::string string );
	// 		~exceptionInvalidArgument() throw(); // err; fixed: exception specification of overriding function is more lax than base version
	// 		virtual const char	*what() const throw();
	// };
/* ------------------------- */
/* -------------------------------------------------------------------------- */
};

#endif // Server_HPP