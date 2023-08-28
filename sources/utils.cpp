#include "../includes/Server.hpp"

/* _________________________ UTILS __________________________________________ */
bool			Server::check( int argc ){
	std::cout << YELLOW "Checking parameters..." END << std::endl;
	if (argc != 3)
		throw( std::invalid_argument(RED "Error: Invalid Argument Count\n"
		"Usage: './ircserv <port> <password>'" END) );
	return (true);
}

unsigned short	Server::port( std::string port ){

	// for: ' $> ./ircserv "" 1234 '
	//		' $> ./ircserv '' 1234 '
	if (port.empty())
		throw( std::invalid_argument(RED "Error: Invalid Port\n"
		"Port cannot be empty." END) );

	for (size_t i = 0; i < port.length(); ++i){
		if (!isdigit(port[i])){
			throw( std::invalid_argument(RED "Error: Invalid Port\n"
			"Port must be a integer." END) );
		}
	}

	unsigned short portVal = atoi(port.c_str());

	// Port numaraları, özel ayrılmış bazı portlar dışında genellikle 0 ile 65535 arasında değer alır.
	//	Ancak, 0 ile 1023 arasındaki port numaraları "well-known ports" olarak adlandırılır ve genellikle sistem servisleri veya tanınmış protokoller tarafından kullanılır.
	//	Bu port numaralarının birçoğu belirli bir protokol veya servis tarafından rezerve edilmiştir.
	//	Bu nedenle, kullanıcı tarafından belirlenecek port numaralarının genellikle 1024'den başlaması tercih edilir. 
	if (portVal < 1024 || portVal > std::numeric_limits<unsigned short>::max())
		throw( std::invalid_argument(RED "Error: Invalid Port\n"
		"Port must be between 1024 and 65535." END) );
	return (portVal);
}

std::string		Server::password( std::string password ){

	// for: ' $> ./ircserv 1234 "" '
	//		' $> ./ircserv 1234 '' '
	if (password.empty())
		throw( std::invalid_argument(RED "Error: Invalid Password\n"
		"Password cannot be empty." END) );
	std::cout << GREEN "Parameters okay." END << std::endl;
	return (password);
}

void	Server::addToPollfds( int fd,  short events, short revents )
{
	pollfd newPoll;

	newPoll.fd = fd;
	newPoll.events = events;
	newPoll.revents = revents;
	this->_pollfds.push_back(newPoll);
}

void	Server::initCommands( void )
{
	t_cmdFunc["CAP"] = &Server::cap;
	t_cmdFunc["JOIN"] = &Server::join;
	t_cmdFunc["NICK"] = &Server::nick;
	t_cmdFunc["PASS"] = &Server::pass;
	t_cmdFunc["QUIT"] = &Server::quit;

	// t_cmdFunc["PRIVMSG"] = &Server::privmsg;
	// t_cmdFunc["USER"] = &Server::user;
	// t_cmdFunc["MODE"] = &Server::mode;
	// t_cmdFunc["PING"] = &Server::ping;

	// t_cmdFunc["KICK"] = &Server::kick;
	// t_cmdFunc["PART"] = &Server::part;
	// t_cmdFunc["WHO"] = &Server::who;
}

std::vector<std::string>	Server::splitMessage( std::string message )
{
	std::stringstream			ss(message);
	std::string					token;
	std::vector<std::string>	tokenArr;

	std::cout << "Message:>" << message << "<" << std::endl;

	while (ss >> token)
		tokenArr.push_back(token);

	// Print tokens.
	std::cout << B_BLUE << "Tokens:>";
	for (size_t i = 0; i < tokenArr.size(); i++)
		std::cout << "`" << tokenArr[i] << "`";
	std::cout << "<" << END << std::endl;
	return (tokenArr);
}

// std::map<std::string, std::string> Server::splitMessage( std::string message )
// {
// 	std::string delimeter = "\r\n";
// 	std::map<std::string, std::string> tokens;
// 	size_t pos = 0;
// 	// CAP LS\r\nNICK gsever\r\nUSER A B C D E F G asdf\r\n
// 	while ((pos = message.find(delimeter)) != std::string::npos)
// 	{
// 		int posFirst = message.find(' ');
// 		// 1. Kısım: USER 2. Kısım: A B C D E F G asdf
// 		//posFirst + 1; CAP'ten sonra gelene boşluğun indexi
// 		//pos - posFirst - 1; "CAP LS\r\n" yapısında LS'i almak için LS'in uzunluğuna bulmaya ihtiyaç var, -1 ise \r'ı almak istemiyoruz.
// 		tokens.insert(std::make_pair(message.substr(0, posFirst), message.substr(posFirst + 1, pos - posFirst - 1)));
// 		message.erase(0, pos + delimeter.length());
// 	}
// 	// mesaj boş olarak geliyor. Kontrol için yapılandırılabilir.
// 	if (!message.empty())
// 		tokens.insert(std::make_pair("UNKNOWN", message));
// 	return (tokens);
// }

std::string Server::trim(const std::string& str)
{
	int	start;
	int	end;

	start = 0;
	while (str[start] != '\0' && str[start] == ' ')
		start++;
	end = str.length() - 1;
	while (end >= 0 && str[end] == ' ')
		end--;

	return (str.substr(start, end));
}

int	Server::findClientName( std::string name )
{
	for (itClients it = this->_clients.begin(); it != _clients.end(); it++)
	{
		if (!it->second->getNickname().compare(name))
			return (it->second->getFd());
		else
			return (-1);
	}
	return (-1);
}

std::string	Server::welcomeServer( void )
{
	std::string	msg;

	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡠⠖⠚⠉⠉⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣶⠟⠋⠒⢦⡀⠀⠻⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡼⢻⠃⠀";
	msg += RED;
	msg += "⣶⠆";
	msg += END;
	msg += "⠀⣷⠖⠒⠢⣌⠉⠉⠒⠒⠂⢤⣄⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡾⠁⠈⢧⠀⠀⠀⢸ ";
	msg += RED;
	msg += "⣶⡄";
	msg += END;
	msg += "⠀⠀⢳⡀⡀⠀⢀⡼⠃⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⢠⡶⣦⠀⠀⠀⠀⠀⠀⢸⠁⠀⠀⠀⠙⠒⣶⣾⡄  ⠀⠀⢨⡇⣷⣴⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⣴⢻⣻⠿⠗⠛⠛⠉⠉⠉⠓⠒⠦⢤⣀⠀⠀⠠⠴⢿⡄⠙⠦⣤⣤⠤⠊⠐⠁⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⣼⣿⣿⡹⡄⣀⡤⠤⠤⠤⠤⢤⣀⡀⠀⠈⣿⣦⡀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠹⣿⠈⠻⣏⠀⠀⠀⠀⠀⠀⠀⠀⠉⠓⢤⣿⢙⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠹⣆⠀⠀⠙⠲⠦⢤⣤⣀⣀⣀⣀⣠⣼⠛⣿⢹⡇⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⢸⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠙⢷⣄⠀⠀⠀⠀⠀⠀⠀⣴⡞⠓⠛⠀⠘⢿⣄⡀⠀⢀⣠⡾⠃⠀⠀⠀⠀⠈⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠉⠓⠦⣤⣀⣀⠀⠀⣨⣷⠀⠀⠀⠀⠀⠈⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⢹⡄⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⣿⠛⠿⣤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠀⠀⠀⠀⠈⣷⠀⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠉⠛⠦⣄⣀⣀⣀⣠⠴⠚⠁⠀⠀⠀⠀⠀⢹⡇⠀⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡶⣿⠀⠀⠀⠀⠀⠀⠀⠀⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⡄⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢾⠀⢿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⠞⠛⠛⠷⠀⠀⠀⠀⠈⣿⠀⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣧⣸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡃⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⡇⠀⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⣾⢋⠉⠛⠛⢷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠻⢶⣄⡀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠻⣿⡇⠀⠀⠀⠙⣦⣀⠀⠀⠀⠀⢀⣤⠤⠼⠿⣦⡄⠀⠀⠀⠀⠀⣶⣦⠀⠀⠉⢻⣄⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⢤⣤⡶⢿⣿⣬⡓⠶⠤⣤⣾⣥⠆⠀⠀⠀⠀⠀⠀⠀⢀⣴⠏⢸⡇⠀⠀⠀⣿⡀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠑⢦⡉⠻⠟⠳⡾⠛⠛⠿⣤⣇⣀⣀⣀⣤⠴⠞⠋⣁⣤⠞⠁⠀⠀⢀⣿⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣇⠀⠀⠀⠀⠉⠛⠛⠛⠛⠛⠛⠋⠉⠀⠀⠀⠀⠀⣼⠏⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⡾⠋⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠓⠦⣤⣄⣀⣀⣀⣀⣀⣤⣤⣤⡶⠾⠛⠉⠀⠀⠀⠀\n";
	msg += " ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
	return (msg);
}
/* -------------------------------------------------------------------------- */