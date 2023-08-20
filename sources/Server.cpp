/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaraca <akaraca@student.42.tr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:29:23 by gsever            #+#    #+#             */
/*   Updated: 2023/08/20 13:46:51 by akaraca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server( int argc, char **argv )
	: _port(static_cast<unsigned short>(std::atoi(argv[1]))),
	_password(argv[2])
{
	(void)argc;
}

Server::~Server( void ) {}

/* _________________________ MAIN FUCTION ___________________________________ */
void	Server::start( void )
{
	std::cout << "Server's first step." << std::endl;
	std::cout << "_port: " << this->_port << " _password: " << this->_password\
		<< std::endl;
}
/* -------------------------------------------------------------------------- */
/* _________________________ SET/GET FUNCTIONS ______________________________ */
/* -------------------------------------------------------------------------- */
