/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-12 09:01:09
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-12 11:17:51
 * @FilePath: /muduo-demo/muduo-tool/mCurrentThread.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "mCurrentThread.h"
#include <sys/syscall.h>
#include <sys/types.h>
#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
namespace mmuduo
{
    namespace mCurrentthread
    {
#ifdef __linux__
        __thread int t_cachedTid = 0;
#elif __APPLE__
        thread_local unsigned int t_cachedTid = 0;
#else
        __thread unsigned int t_cachedTid = 0;
#endif
        void cachTid()
        {
            if (t_cachedTid == 0)
            {
                // t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
            }
        }
    }
}