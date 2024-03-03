#include "mInetAddress.h"
#include <logger.h>
using namespace mmuduo;
using namespace mnet;

mmuduo::mnet::mInetAddress::mInetAddress(uint16_t port, std::string ip, bool isIvp6)
{
    this->_ip = ip;
    this->_port = port;
    //  ivp6
    if (isIvp6 == true)
    {
        memset(&addr6_, 0, sizeof(addr6_));
        addr6_.sin6_family = AF_INET6;
        if (inet_pton(AF_INET6, ip.c_str(), &addr6_) <= 0)
        {
            LOG_ERR("Ivp6 inet_pton failed");
        }
        addr6_.sin6_port = htons(port);
        return;
    }
    //  ivp4
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    if (inet_pton(AF_INET, ip.c_str(), &addr_) <= 0)
    {
        LOG_ERR("Ivp4 inet_pton failed");
    }
    addr_.sin_port = htons(port);
}

mmuduo::mnet::mInetAddress::mInetAddress(const sockaddr_in &addr)
{
    char paresIp[32] = {0};
    if (inet_ntop(AF_INET, &addr, paresIp, sizeof(addr)) == NULL)
    {
        LOG_ERR("mInetAddress::mInetAddress(const sockaddr_in &addr)->inet_ntop failed, reason :", errno);
    }
    this->_ip = paresIp;
    this->_ip = htons(addr.sin_port);
}

mmuduo::mnet::mInetAddress::mInetAddress(const sockaddr_in6 &addr)
{
    char paresIp[128] = {0};
    if (inet_ntop(AF_INET6, &addr, paresIp, sizeof(addr)) == NULL)
    {
        LOG_ERR("mInetAddress::mInetAddress(const sockaddr_in6 &addr)->inet_ntop failed, reason :", errno);
    }
    this->_ip = paresIp;
    this->_ip = htons(addr.sin6_port);
}

std::string mmuduo::mnet::mInetAddress::toIp() const
{
    return _ip;
}

std::string mmuduo::mnet::mInetAddress::toIpPort() const
{
    return _ip + ":" + _port;
}

std::string mmuduo::mnet::mInetAddress::toPort() const
{
    return _port;
}

const sockaddr_in *mmuduo::mnet::mInetAddress::getSockAddr() const
{
    return &addr_;
}

mmuduo::mnet::mInetAddress::~mInetAddress()
{
}
