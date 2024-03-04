/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-04 08:49:37
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-04 10:02:59
 * @FilePath: /muduo-demo/test/testInet.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <mInetAddress.h>

using namespace std;
using namespace mmuduo;
using namespace mnet;
int main(void)
{
    mInetAddress addr(8080);
    cout << addr.toIpPort() << endl;
    cout << addr.toIp() << endl;
    cout << addr.toPort() << endl;
    cout << addr.getSockAddr() << endl;
}
