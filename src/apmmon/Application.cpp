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

#include <Application.hpp>
#include <cstdlib>
#include <Exceptions.hpp>
#include <future>

namespace apm {

Application::Application(std::ostream& _outputStream)
    : outputStream(_outputStream)
    , apmCalculator()
    , apmClassifier()
    , csvFormatter()
    , eventSerializer(csvFormatter, outputStream)
    , eventQueue()
    , eventMonitor(eventQueue)
    , eventMultiplexer(eventQueue)
{
    eventMultiplexer.addObserver(&apmClassifier);
    apmClassifier.addObserver(&apmCalculator);
    apmCalculator.addObserver(&eventSerializer);
}

Application::~Application()
{
    apmCalculator.removeObserver(&eventSerializer);
    apmClassifier.removeObserver(&apmCalculator);
    eventMultiplexer.removeObserver(&apmClassifier);
}

int
Application::run()
{
    try
    {
        // clang-format off
        auto eventMultiplexerExceptions = std::async(std::launch::async, [this]
        {
            eventMultiplexer.run();

        });
        // clang-format on
        eventMonitor.run();
        eventMultiplexerExceptions.get();

        return EXIT_SUCCESS;
    }
    catchAndLogAll(return EXIT_FAILURE;)
}

} // namespace apm
