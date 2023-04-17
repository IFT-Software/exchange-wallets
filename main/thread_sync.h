#ifndef THREAD_SYNC_H 
#define THREAD_SYNC_H

#include <condition_variable>
#include <mutex>

/**
 * @brief A thread synchronization that contains all of the locks, mutexes and condition variables needed to protect 
 * shared resources
 * 
 */

class ThreadSync {

public:
  std::mutex database_mutex;
  std::mutex mempool_mutex;

  std::condition_variable mempool_cv;

  ThreadSync() = default;
};


#endif