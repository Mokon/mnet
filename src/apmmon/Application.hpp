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

#include <classifiers/ApmClassifier.hpp>
#include <EventMonitor.hpp>
#include <EventMultiplexer.hpp>
#include <EventQueue.hpp>
#include <formatters/CsvFormatter.hpp>
#include <observers/ApmCalculator.hpp>
#include <observers/EventSerializer.hpp>
#include <iostream>

namespace apm {

class Application final
{
  public:
    Application(std::ostream& _outputStream = std::cout);
    ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&)                 = delete;
    Application& operator=(Application&&) = delete;

  public:
    int run();

    template <class... Args>
    decltype(auto) postEvent(Args&&... args)
    {
        return eventQueue.emplace(std::forward<Args...>(args...));
    }

  private:
    std::ostream&    outputStream;
    ApmCalculator    apmCalculator;
    ApmClassifier    apmClassifier;
    CsvFormatter     csvFormatter;
    EventSerializer  eventSerializer;
    EventQueue       eventQueue;
    EventMonitor     eventMonitor;
    EventMultiplexer eventMultiplexer;
};

} // namespace apm
