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

#include <chrono>
#include <EventQueue.hpp>
#include <list>
#include <windows.h>

namespace apm {

class Hooks final
{
  public:
    static Hooks* getInstance();
    void          setEventQueue(EventQueue& _eventQueue);

  private:
    Hooks();
    ~Hooks();
    Hooks(const Hooks&) = delete;
    Hooks& operator=(const Hooks&) = delete;
    Hooks(Hooks&&)                 = delete;
    Hooks& operator=(Hooks&&) = delete;

  public:
    void addHook(int hookId, HOOKPROC callback);

  private:
    static LRESULT CALLBACK
                   keyboardHook(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK mouseHook(int nCode, WPARAM wParam, LPARAM lParam);

    void processEvent(KBDLLHOOKSTRUCT event);

  private:
    std::list<HHOOK> hooks;
    EventQueue*      eventQueue;
    TimePoint        bootTime;
};

} // namespace apm
