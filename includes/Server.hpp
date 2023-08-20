/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsever <gsever@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:26:25 by gsever            #+#    #+#             */
/*   Updated: 2023/08/20 15:33:11 by gsever           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstdlib>
# include "Colors.hpp"
# include "Utils.hpp"

class Server
{
	private:
		const uint16_t		_port;
		const std::string	_password;
		Server( void ); // Default Constructor.
	public:
		Server( int argc, char **argv );
		~Server( void ); // Destructor.
/* _________________________ MAIN FUCTION ___________________________________ */
		void	start( void );
/* -------------------------------------------------------------------------- */
/* _________________________ SET/GET FUNCTIONS ______________________________ */
/* -------------------------------------------------------------------------- */
/* _________________________ TIME FUNCTIONS _________________________________ */
/* -------------------------------------------------------------------------- */
/* _________________________ IF FUNCTIONS ___________________________________ */
/* -------------------------------------------------------------------------- */
/* _________________________ SORTING FUNCTIONS ______________________________ */
/* -------------------------------------------------------------------------- */
/* _________________________ PRINT FUCTIONS _________________________________ */
/* -------------------------------------------------------------------------- */
/* _________________________ EXCEPTIONS _____________________________________ */

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