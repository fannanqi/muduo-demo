/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-03-12 08:59:46
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-03-12 15:51:26
 * @FilePath: /muduo-demo/muduo-tool/include/mCurrentThread.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
namespace mmuduo
{
    namespace mCurrentthread
    {
#ifdef __linux__
        extern __thread int t_cachedTid;
#elif __APPLE__
        extern thread_local unsigned int t_cachedTid;
#else
        extern __thread unsigned int t_cachedTid;
#endif
        void cachTid();

        inline int tid()
        {
            if (__builtin_expect(t_cachedTid == 0, 0))
            {
                cachTid();
            }
            return t_cachedTid;
        }
    }
}