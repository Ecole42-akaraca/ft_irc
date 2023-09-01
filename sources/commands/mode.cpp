#include "../../includes/Server.hpp"

/**
 * @brief 
 * 
 * @link https://www.antionline.com/showthread.php?136933-IRC-flags
 * @link https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.5
 * @link https://irssi.org/documentation/help/mode/
 * 
 * PRIVMSG
Message:>MODE-naber<
Message:>naber<
Tokens:>`naber`<
MODE
Message:>MODE-#naber<
Message:>#naber<
Tokens:>`#naber`<
MODE
Message:>MODE-naber asdf<
Message:>naber asdf<
Tokens:>`naber``asdf`<
MODE
Message:>MODE-#naber +asdf<
Message:>#naber +asdf<
Tokens:>`#naber``+asdf`<
MODE
Message:>MODE-#naber +asdf<
Message:>#naber +asdf<
Tokens:>`#naber``+asdf`<
MODE
Message:>MODE-#naber -asdf<
Message:>#naber -asdf<
Tokens:>`#naber``-asdf`<
MODE
Message:>MODE-naber -asdf<
Message:>naber -asdf<
Tokens:>`naber``-asdf`<
MODE
Message:>MODE-naber -asdf asdf asdf<
Message:>naber -asdf asdf asdf<
Tokens:>`naber``-asdf``asdf``asdf`<
MODE
Message:>MODE-#naber -asdfasdfasdf<
Message:>#naber -asdfasdfasdf<
Tokens:>`#naber``-asdfasdfasdf`<
MODE
Message:>MODE-#naber +asdfasdfasdfasdfasfasdf<
Message:>#naber +asdfasdfasdfasdfasfasdf<
Tokens:>`#naber``+asdfasdfasdfasdfasfasdf`<
MODE
^C

 * @link https://matrix-org.github.io/matrix-appservice-irc/latest/irc_modes.html
 * 
 * @param message 
 */
void	Server::mode( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "MODE" << END << std::endl;
	std::cout << "tokenarr[0]: " << tokenArr[0] << std::endl;
	if (tokenArr.size() < 2)
	{
		it->sendMessageFd(ERR_NEEDMOREPARAMS(it->getNickname(), tokenArr[0]));
		return;
	}

	// #define RPL_MODE(source, channel, modes, args)	":" + source + " MODE " + channel + " " + modes + " " + args
	// it->sendMessageFd(RPL_MODE(it->getPrefix(), it->get));
}