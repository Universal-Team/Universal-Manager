/* This File is part of Universal-Upater!
* Diese Datei ist Teil von Universal-Updater!
 */

#include <string.h>
#include <stdio.h>
#include <3ds.h>
#include "thread.hpp"

static std::vector<Thread> threads;

void Threads::create(ThreadFunc entrypoint)
{
    s32 prio = 0;
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    Thread thread = threadCreate((ThreadFunc)entrypoint, NULL, 4*1024, prio-1, -2, false);
    threads.push_back(thread);
}

void Threads::destroy(void)
{
    for (u32 i = 0; i < threads.size(); i++)
    {
        threadJoin(threads.at(i), U64_MAX);
        threadFree(threads.at(i));
    }
    threads.clear();
}
