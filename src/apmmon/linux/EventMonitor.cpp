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
#include <EventMonitor.hpp>
#include <linux/ExitHandler.hpp>
#include <linux/InputDeviceCollector.hpp>
#include <linux/InputEventReader.hpp>
#include <linux/InputEventClassifier.hpp>
#include <linux/Poller.hpp>
#include <linux/PrivilegeHandler.hpp>
#include <linux/ScopedPrivilege.hpp>

namespace apm {

void
EventMonitor::run()
{
    std::vector<Privilege> allowedPrivileges({Privilege::Input});
    PrivilegeHandler       privilegeHandler{allowedPrivileges};
    ExitHandler*           exitHandler = ExitHandler::getInstance();
    assert(exitHandler != nullptr);
    exitHandler->setEventMonitor(*this);

    InputEventClassifier classifier;
    InputDeviceCollector idc;
    InputDevices         devices;
    {
        ScopedPrivilege inputPrivilege(privilegeHandler, Privilege::Input);
        devices = idc.collectAllKeyboardsAndMice();
    }
    Poller           poller{devices};
    InputEventReader reader;

    while (poller.hasInputPending())
    {
        while (poller.hasMore())
        {
            reader.read(poller.next());
            while (reader.hasMore())
            {
                Event event{classifier.get(reader.next())};
                if (not event.shouldIgnore())
                {
                    eventQueue.emplace(event);
                }
            }
        }
    }
}

} // namespace apm
