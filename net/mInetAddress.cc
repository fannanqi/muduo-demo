/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-04 08:30:17
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-04 21:50:02
 * @FilePath: /muduo-demo/net/mInetAddress.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "mInetAddress.h"
#include <logger.h>
#include <errno.h>
using namespace mmuduo;
using namespace mmuduo::mnet;

mInetAddress::mInetAddress(uint16_t port, std::string ip, bool isIvp6)
{
    this->_ip = ip;
    this->_port = std::to_string(port);
    //  ivp6
    if (isIvp6 == true)
    {
        memset(&addr6_, 0, sizeof(addr6_));
        addr6_.sin6_family = AF_INET6;
        if (inet_pton(AF_INET6, ip.c_str(), &addr6_) <= 0)
        {
            LOG_ERR("Ivp6 inet_pton failed,reason is %s", strerror(errno));
        }
        addr6_.sin6_port = htons(port);
        return;
    }
    //  ivp4
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    if (inet_pton(AF_INET, ip.c_str(), &addr_) <= 0)
    {
        LOG_ERR("Ivp4 inet_pton failed,reason is %s", strerror(errno));
    }
    addr_.sin_port = htons(port);
}

mInetAddress::mInetAddress(const sockaddr_in &addr)
{
    char paresIp[32] = {0};
    if (inet_ntop(AF_INET, &addr, paresIp, sizeof(addr)) == NULL)
    {
        LOG_ERR("mInetAddress::mInetAddress(const sockaddr_in &addr)->inet_ntop failed,reason is %s", strerror(errno));
    }
    this->_ip = paresIp;
    this->_ip = htons(addr.sin_port);
}

mInetAddress::mInetAddress(const sockaddr_in6 &addr)
{
    char paresIp[128] = {0};
    if (inet_ntop(AF_INET6, &addr, paresIp, sizeof(addr)) == NULL)
    {
        LOG_ERR("mInetAddress::mInetAddress(const sockaddr_in6 &addr)->inet_ntop failed,reason is %s", strerror(errno));
    }
    this->_ip = paresIp;
    this->_ip = htons(addr.sin6_port);
}

std::string mInetAddress::toIp() const
{
    return _ip;
}

std::string mInetAddress::toIpPort() const
{
    return _ip + ":" + _port;
}

std::string mInetAddress::toPort() const
{
    return _port;
}

const sockaddr_in *mInetAddress::getSockAddr() const
{
    return &addr_;
}

mInetAddress::~mInetAddress()
{
}
