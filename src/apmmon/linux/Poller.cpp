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

#include <cassert>
#include <linux/Poller.hpp>
#include <unistd.h>

namespace apm {

Poller::Poller(const InputDevices& _devices)
    : devices(_devices)
    , fd(epoll_create1(EPOLL_CLOEXEC))
    , currentEventIndex(0)
    , currentEventMax(0)
{
    if (not isOpen())
    {
        throw std::system_error(errno, std::generic_category());
    }

    for (auto& device : devices)
    {
        if (epoll_ctl(fd, EPOLL_CTL_ADD, device.data.fd, &device) < 0)
        {
            throw std::system_error(errno, std::generic_category());
        }
    }
}

Poller::~Poller()
{
    close();
}

bool
Poller::hasInputPending()
{
    int noTimeout   = -1;
    currentEventMax = epoll_wait(fd, events, maxEventsPerCycle, noTimeout);
    if (currentEventMax < 0)
    {
        if (errno != EINTR)
        {
            throw std::system_error(errno, std::generic_category());
        }
        else
        {
            return false;
        }
    }

    currentEventIndex = 0;
    return true;
}

bool
Poller::hasMore() const
{
    return currentEventIndex < currentEventMax;
}

int
Poller::next()
{
    assert(hasMore());
    return events[currentEventIndex++].data.fd;
}

bool
Poller::isOpen() const
{
    return fd != -1;
}

void
Poller::close()
{
    if (isOpen())
    {
        if (::close(fd) < 0)
        {
            std::perror("poller close failed");
        }
        fd = -1;
    }
}

} // namespace apm
