/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsever <gsever@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:08:12 by gsever            #+#    #+#             */
/*   Updated: 2023/08/20 16:19:25 by gsever           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"
# include "../includes/Utils.hpp"

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
 * @param argc: Server port number.
 * @param argv: Server password.
 * @return int 
 */
int	main( int argc, char **argv )
{
	try
	{
		Utils::validateArguments(argc, argv);
		Server server(argc, argv);
		server.start();
		std::cout << B_GREEN "IRC Server started!" END << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_SUCCESS);
	}
}