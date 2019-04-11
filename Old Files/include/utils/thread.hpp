

#ifndef THREAD_HPP
#define THREAD_HPP

#include <vector>

namespace Threads
{
    void create(ThreadFunc entrypoint);
    void destroy(void);
}

#endif
