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

#include <fcntl.h>
#include <linux/InputDevice.hpp>
#include <unistd.h>

namespace apm {

InputDevice::InputDevice(const fs::path& _file)
    : InputDevice(fs::directory_entry(_file))
{
}

InputDevice::InputDevice(const fs::directory_entry& _file)
    : file(_file)
    , fd(open(file.path().c_str(), O_RDONLY))
{
    if (isValid())
    {
        setSupportedEvents();
        if (supportsKeyboardEvents())
        {
            setSupportedKeys();
        }
    }
}

InputDevice::~InputDevice()
{
    close();
}

bool
InputDevice::isValid() const
{
    return isOpen() and file.is_character_file();
}

bool
InputDevice::isKeyboard() const
{
    return supportsKeys(KEY_A | KEY_B | KEY_C | KEY_Z);
}

struct epoll_event
InputDevice::getEpollEvent()
{
    struct epoll_event event;
    event.events  = EPOLLIN;
    event.data.fd = fd;
    fd            = -1;
    return event;
}

bool
InputDevice::isOpen() const
{
    return fd != -1;
}

void
InputDevice::close()
{
    if (isOpen())
    {
        if (::close(fd) < 0)
        {
            std::perror("input device close failed");
        }
        fd = -1;
    }
}

void
InputDevice::setSupportedEvents()
{
    if (ioctl(fd, EVIOCGBIT(0, sizeof(supportedEvents)), &supportedEvents) < 0)
    {
        if (errno != ENOTTY)
        {
            throw std::system_error(errno, std::generic_category());
        }
    }
}

bool
InputDevice::supportsKeyboardEvents() const
{
    return (EV_KEY & supportedEvents) == EV_KEY;
}

void
InputDevice::setSupportedKeys()
{
    if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(supportedKeys)), &supportedKeys) < 0)
    {
        throw std::system_error(errno, std::generic_category());
    }
}

bool
InputDevice::supportsKeys(int32_t keys) const
{
    return (keys & supportedKeys) == keys;
}

} // namespace apm
