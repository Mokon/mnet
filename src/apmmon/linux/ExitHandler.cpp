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
#include <linux/ExitHandler.hpp>

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
    for (auto sig : signals)
    {
        registerSignal(sig);
    }
}

ExitHandler::~ExitHandler()
{
    for (auto sig : signals)
    {
        unregisterSignal(sig);
    }
}

void
ExitHandler::registerSignal(int sig)
{
    struct sigaction action
    {
    };
    action.sa_sigaction = &signalHandler;
    action.sa_flags     = SA_SIGINFO;

    if (sigaction(sig, &action, nullptr) < 0)
    {
        throw std::system_error(errno, std::generic_category());
    }
}

void
ExitHandler::unregisterSignal(int sig)
{
    struct sigaction action
    {
    };
    action.sa_handler = SIG_DFL;

    if (sigaction(sig, &action, nullptr) < 0)
    {
        std::perror("failed to unregister signal");
    }
}

void
ExitHandler::signalHandler(int sig, siginfo_t* siginfo, void* context)
{
    (void)sig;
    (void)siginfo;
    (void)context;
    assert(getInstance() != nullptr);

    if (getInstance()->eventMonitor.load() != nullptr)
    {
        getInstance()->eventMonitor.load()->stop();
    }
}

} // namespace apm
