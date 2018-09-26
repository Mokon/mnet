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
#include <Diagnostics.hpp>
#include <iostream>
#include <sstream>
#include <windows/ExitHandler.hpp>

namespace apm {

ExitHandler*
ExitHandler::getInstance()
{
    allowExitTimeDestruction(static ExitHandler instance;);
    return &instance;
}

void
ExitHandler::setEventMonitor(EventMonitor& _eventMonitor)
{
    assert(eventMonitor == nullptr);
    eventMonitor = &_eventMonitor;
}

ExitHandler::ExitHandler()
{
    if (not SetConsoleCtrlHandler(
            static_cast<PHANDLER_ROUTINE>(consoleCtrl), true))
    {
        std::stringstream error;
        error << "failed to add console ctrl handler: " << GetLastError();
        throw std::runtime_error(error.str());
    }
}

ExitHandler::~ExitHandler()
{
    if (not SetConsoleCtrlHandler(nullptr, false))
    {
        std::cerr << "failed to remove console ctrl handler " << GetLastError()
                  << '\n';
    }
}

BOOL WINAPI
     ExitHandler::consoleCtrl(DWORD dwType)
{
    switch (dwType)
    {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
            assert(getInstance() != nullptr);
            assert(getInstance()->eventMonitor != nullptr);
            getInstance()->eventMonitor->stop();
            PostQuitMessage(0);
            break;
        default:
            break;
    }
    return true;
}

} // namespace apm
