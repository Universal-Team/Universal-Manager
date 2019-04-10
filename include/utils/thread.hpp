/* This File is part of Universal-Upater!
* Diese Datei ist Teil von Universal-Updater!
 */

#ifndef THREAD_HPP
#define THREAD_HPP

#include <vector>

namespace Threads
{
    void create(ThreadFunc entrypoint);
    void destroy(void);
}

#endif
