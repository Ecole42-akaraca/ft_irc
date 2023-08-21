/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaraca <akaraca@student.42.tr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:08:12 by gsever            #+#    #+#             */
/*   Updated: 2023/08/20 18:04:52 by akaraca          ###   ########.fr       */
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
 * istemci ornek:	telnet localhost 8080
 * 					nc localhost  8080
 * 					url olarak: localhost:8080
 * 
 * izlemek için(Mac'te): nettop -m tcp	
 * 
 * IRC url:	https://www.youtube.com/watch?v=JpLwB14L2Rk&ab_channel=TallPaulTech
 * 			https://www.youtube.com/watch?v=njKv66miR80&list=PL5tDlMcZytRqvYbDWLoayxAkUcHsOxX_p&index=9
 * 
 * 			https://beej.us/guide/bgnet/html/split/
 * 
 * @param argc: Server port number.
 * @param argv: Server password.
 * @return int 
 */
int	main( int argc, char **argv )
{
	try
	{
		Server Server(argc, argv);
		Server.start();
		std::cout << B_GREEN "IRC Server started!" END << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_SUCCESS);
	}
}
