/*
 * Copyright (c) 2018 David 'Mokon' Bond <mokon@mokon.net>
 * Author: David 'Mokon' Bond <mokon@mokon.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <events/Event.hpp>

#if __has_include(<tbb/concurrent_queue.h>)
#include <tbb/concurrent_queue.h>
#else
#pragma message "tbb not found, using locking"

#include <mutex>
#include <condition_variable>
#include <queue>
#endif

namespace apm {

#if __has_include(<tbb/concurrent_queue.h>)
using EventQueue = [[threadsafe]] tbb::concurrent_bounded_queue<Event>;
#else

class [[threadsafe]] EventQueue
{
  public:
    EventQueue()                  = default;
    ~EventQueue()                 = default;
    EventQueue(const EventQueue&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;
    EventQueue(EventQueue &&)                = delete;
    EventQueue& operator=(EventQueue&&) = delete;

  public:
    void pop(Event & event)
    {
        std::unique_lock<std::mutex> guard(mutex);
        pending.wait(guard, [this] { return not queue.empty(); });
        event = Event(queue.front());
        queue.pop();
    }

    bool try_pop(Event & event)
    {
        std::unique_lock<std::mutex> guard(mutex);
        if (queue.empty())
        {
            return false;
        }
        else
        {
            event = Event(queue.front());
            queue.pop();
            return true;
        }
    }

    template <class... Args>
    decltype(auto) emplace(Args && ... args)
    {
        std::lock_guard<std::mutex> guard(mutex);
        auto ret = queue.emplace(std::forward<Args...>(args...));
        pending.notify_one();
        return ret;
    }

    size_t size()
    {
        std::lock_guard<std::mutex> guard(mutex);
        return queue.size();
    }

  private:
    std::mutex              mutex;
    std::condition_variable pending;
    std::queue<Event>       queue;
};

#endif

} // namespace apm
