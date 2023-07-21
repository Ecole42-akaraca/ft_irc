/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsever <gsever@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:08:12 by gsever            #+#    #+#             */
/*   Updated: 2023/07/21 23:01:19 by gsever           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCServ.hpp"

int	main( int argc, char **argv )
{
	std::cout << "IRC Server started!" << std::flush << std::endl;
	IRCServ	server(argc, argv);
	return (EXIT_SUCCESS);
}