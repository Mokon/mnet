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
#include <windows/Hooks.hpp>

namespace apm {

Hooks*
Hooks::getInstance()
{
    allowExitTimeDestruction(static Hooks instance;);
    return &instance;
}

void
Hooks::setEventQueue(EventQueue& _eventQueue)
{
    assert(eventQueue == nullptr);
    eventQueue = &_eventQueue;
}

Hooks::Hooks()
    : bootTime(Time::now() - std::chrono::milliseconds(GetTickCount()))
// TODO this is not exzact boot time
{
    addHook(WH_KEYBOARD_LL, keyboardHook);
    addHook(WH_MOUSE_LL, mouseHook);
}

Hooks::~Hooks()
{
    for (auto hook : hooks)
    {
        if (not UnhookWindowsHookEx(hook))
        {
            std::cerr << "failed to remove hook " << GetLastError() << '\n';
        }
    }
}

void
Hooks::addHook(int hookId, HOOKPROC callback)
{
    auto hook = SetWindowsHookEx(hookId, callback, nullptr, 0);
    if (hook == nullptr)
    {
        std::stringstream error;
        error << "failed to add hook " << hookId << ":" << GetLastError();
        throw std::runtime_error(error.str());
    }
    hooks.push_back(hook);
}

void
Hooks::processEvent(KBDLLHOOKSTRUCT event)
{
    LONG code = 1 + static_cast<LONG>(event.scanCode << 16) +
                static_cast<LONG>(event.flags << 24);

    char raw[MAX_PATH] = {0};
    GetKeyNameText(code, raw, MAX_PATH);

    // TODO classify keyboard events
    eventQueue->emplace(Event(
        bootTime + std::chrono::milliseconds(event.time),
        Key::Other,
        KeyModifier::Unknown,
        raw));
}

LRESULT CALLBACK
        Hooks::keyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION and
        (wParam == WM_SYSKEYDOWN or wParam == WM_KEYDOWN))
    {
        KBDLLHOOKSTRUCT event = *reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        assert(getInstance() != nullptr);
        assert(getInstance()->eventQueue != nullptr);
        getInstance()->processEvent(event);
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK
        Hooks::mouseHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    assert(getInstance() != nullptr);
    assert(getInstance()->eventQueue != nullptr);

    // TODO classify mouse events
    // nCode >= 0 wParam == WM_RBUTTONDOWN wParam == WM_RBUTTONUP)
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

} // namespace apm
