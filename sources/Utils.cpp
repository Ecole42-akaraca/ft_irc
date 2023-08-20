#include "../includes/Utils.hpp"

void	Utils::validateArguments(int argc, char **argv){

	std::cout << YELLOW "Checking parameters..." END << std::endl;
	if (argc != 3)
		throw( Utils::exceptionInvalidArgument(RED \
		"Error: Invalid Argument, usage: './ircserv <port> <password>'" END) );	

	const std::string port = argv[1];
	const std::string password = argv[2];

	if (!Utils::isValidPort(port))
		throw( Utils::exceptionInvalidArgument(RED "Error: Invalid Port" END) );

	// for: ' $> ./ircserv 1234 "" '
	// 		' $> ./ircserv 1234 "" '
	if (password.empty())
		throw( Utils::exceptionInvalidArgument(RED "Error: Invalid Password" END) );
}

bool	Utils::isValidPort( const std::string &port ){
	for (size_t i = 0; i < port.length(); ++i){
		if (!isdigit(port[i])){
			return (false);
		}
	}

	int portInt = atoi(port.c_str());

	// Port numaraları, özel ayrılmış bazı portlar dışında genellikle 0 ile 65535 arasında değer alır.
	//	Ancak, 0 ile 1023 arasındaki port numaraları "well-known ports" olarak adlandırılır ve genellikle sistem servisleri veya tanınmış protokoller tarafından kullanılır.
	//	Bu port numaralarının birçoğu belirli bir protokol veya servis tarafından rezerve edilmiştir.
	//	Bu nedenle, kullanıcı tarafından belirlenecek port numaralarının genellikle 1024'den başlaması tercih edilir. 
	// if (portInt < 1024 || portInt > 65535)
	if (portInt < 1024 || portInt > std::numeric_limits<unsigned short>::max())
		return (false);
	return (true);
}
