/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-03 21:27:31
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-03 23:22:58
 * @FilePath: /muduo-demo/net/include/mInetAddress.h
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-03 21:27:31
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-03 21:29:10
 * @FilePath: /muduo-demo/net/InetAddress.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
namespace mmuduo
{
    namespace mnet
    {

        //  封装socketd地址类型
        class mInetAddress
        {
        private:
            sockaddr_in addr_;
            sockaddr_in6 addr6_;
            bool _isIvp6;
            std::string _ip;
            std::string _port;

        public:
            explicit mInetAddress(uint16_t port, std::string ip = "0.0.0.0", bool isIvp6 = false);
            explicit mInetAddress(const sockaddr_in &);
            explicit mInetAddress(const sockaddr_in6 &a);
            //  获取IP信息
            std::string toIp() const;
            //  获取IP+PORT信息
            std::string toIpPort() const;
            //  获取PORT信息
            std::string toPort() const;

            const sockaddr_in *getSockAddr() const;
            ~mInetAddress();
        };

    }
}