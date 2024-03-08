#include "mPoll.h"
#include <mEventLoop.h>
#include <mchannel.h>
using namespace mmuduo;
using namespace mmuduo::mnet;

mPoll::mPoll(mEventLoop *loop)
    : _ownerLoop(loop)
{
}

bool mPoll::hasChannel(mChannel *channel) const
{
    ChannelMap::const_iterator it = _channels.find(channel->fd());
    return it != _channels.end() && it->second == channel;
}
