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

#include <folly/Format.h>
#include <Time.hpp>
#include <test/ApplicationTestSuite.hpp>
#include <test/framework/Testing.hpp>

namespace apm { namespace test {

using EventTestSuite = ApplicationTestSuite;

Test(EventTestSuite, CsvOutputHeader)
{
    expectCsvHeader();
}

Test(EventTestSuite, StartStopApp)
{
    start();
    expectCsvHeader();
    stop();
    expectNoOutput();
}

Test(EventTestSuite, PostEvents)
{
    start();
    expectCsvHeader();

    auto      templateS = "Keyboard, raw, {}, Press, {}, 0\n";
    TimePoint t1{std::chrono::seconds{0}};

    app.postEvent(Event(t1, Key::A, KeyModifier::Press, "raw"));
    expectBecomesOutput(folly::sformat(templateS, "A", Time::toString(t1)));

    TimePoint t2{std::chrono::seconds{1}};

    app.postEvent(Event(t2, Key::B, KeyModifier::Press, "raw"));
    expectBecomesOutput(folly::sformat(templateS, "B", Time::toString(t2)));

    stop();
    expectNoOutput();
}

Test(EventTestSuite, Apm)
{
    start();
    expectCsvHeader();

    auto      templateS = "Keyboard, raw, {}, Press, {}, {}\n";
    TimePoint t1{Time::now()};
    TimePoint t2{t1 + std::chrono::seconds{1}};

    app.postEvent(Event(t1, Key::A, KeyModifier::Press, "raw"));
    expectBecomesOutput(folly::sformat(templateS, "A", Time::toString(t1), 1));

    app.postEvent(Event(t2, Key::A, KeyModifier::Press, "raw"));
    expectBecomesOutput(folly::sformat(templateS, "A", Time::toString(t2), 2));

    stop();
    expectNoOutput();
}

}} // namespace apm::test
