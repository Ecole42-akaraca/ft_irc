#include "../../includes/Server.hpp"

void	Server::pass( Client* it, std::vector<std::string> tokenArr ) // OK
{
	// `PASS``asdf``NICK``yuandre``USER``yuandre``yuandre``localhost``:Görkem``Sever`
	std::cout << YELLOW << "PASS" << END << std::endl;
	if (!tokenArr.empty() && !tokenArr[0].compare("PASS"))
		if (!tokenArr[1].compare(this->_password))
			it->setPasswordStatus();
		else
		{
			tokenArr.clear();
			tokenArr.push_back("Password is wrong.");
			Server::quit(it, tokenArr);
		}
	else
	{
		tokenArr.clear();
		tokenArr.push_back("Password is empty.");
		Server::quit(it, tokenArr);
	}
}