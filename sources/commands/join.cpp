# include "../../includes/Server.hpp"

void	Server::join( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "JOIN" << END << std::endl;
	if (it->getIRCstatus() != AUTHENTICATED)
		return ;
	if (!tokenArr[1].compare("#"))
		tokenArr[1] = "#" + tokenArr[1];

	itChannels itChannel = _channels.find(tokenArr[1]);
	it->sendMessageFd(RPL_JOIN(it->getPrefix(), tokenArr[1]));
	if (itChannel != _channels.end())
	{
		itChannel->second->sendMessageBroadcast(it, RPL_JOIN(it->getPrefix(), tokenArr[1]));
		itChannel->second->setChannelTopic("I have no idea what the topic is.");
		it->sendMessageFd(RPL_TOPIC(it->getPrefix(), tokenArr[1], itChannel->second->getChannelTopic()));
		itChannel->second->addClient(it);
		itChannel->second->channelUsers(it, itChannel->second, tokenArr[1]);
		it->registerChannel(itChannel->second);
	}
	else
	{
		Channel *channel = new Channel(tokenArr[1], "NULL", it);
		channel->addClient(it);
		this->_channels.insert(std::make_pair(tokenArr[1], channel));
		channel->setChannelTopic("What day is it today?");
		it->sendMessageFd(RPL_TOPIC(it->getPrefix(), tokenArr[1], channel->getChannelTopic()));
		it->sendMessageFd(RPL_MODE(it->getNickname(), tokenArr[1], "+nto", it->getNickname()));
		it->registerChannel(channel);
	}
}
