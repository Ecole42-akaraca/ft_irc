/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaraca <akaraca@student.42.tr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:08:12 by gsever            #+#    #+#             */
/*   Updated: 2023/08/20 14:45:10 by akaraca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

/**
 * @brief 
 * 
 * @TODO: Server olusturulduktan sonra client'ten gelen baglanti aninda;
 * Server; baglanan client'in 
 * 'file descriptor',
 * 'address' ve 
 * 'port'unu printle. Bunlari --DEBUG=true flagiyle calistirirken printlesin.
 * ornek: ./ircserv --DEBUG=true <port> <password>
 * istemci ornek: telnet localhost 8080
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int	main( int argc, char **argv )
{
	try
	{
		Utils::validateArguments(argc, argv);
		std::cout << "IRC Server started!" << std::flush << std::endl;
		Server server(argc, argv);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_SUCCESS);
	}
}