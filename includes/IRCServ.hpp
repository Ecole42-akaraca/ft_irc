/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaraca <akaraca@student.42.tr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:26:25 by gsever            #+#    #+#             */
/*   Updated: 2023/08/19 15:08:25 by akaraca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	IRCSERV_HPP
# define IRCSERV_HPP

# include <iostream>
# include <cstdlib>
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