# include "../../includes/Server.hpp"

void	Server::pass( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "PASS" << END << std::endl;
	if (!tokenArr.empty() && !tokenArr[0].compare("PASS"))
		if (!tokenArr[1].compare(this->_password))
			it->setPasswordStatus();
		else
			Server::quitReason(it, "Password is wrong!");
	else
		Server::quitReason(it, "Password is empty!");
}
