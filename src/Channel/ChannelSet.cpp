
#include "irc.hpp"

void Channel::setTopic(const std::string new_topic) {
	if (new_topic.empty())
		return ;
	this->_channel_topic = new_topic;
}