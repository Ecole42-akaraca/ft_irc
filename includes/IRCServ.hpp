/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsever <gsever@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:26:25 by gsever            #+#    #+#             */
/*   Updated: 2023/07/21 23:01:23 by gsever           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	IRCSERV_HPP
# define IRCSERV_HPP

# include <iostream>
# include "Colors.hpp"

class IRCServ
{
	private:
		// int	i;
	public:
	IRCServ( void ); // Default Constructor.
	IRCServ( int argc, char **argv );
	~IRCServ( void ); // Destructor.
/* _________________________ MAIN FUCTION ___________________________________ */
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

#endif // IRCSERV_HPP