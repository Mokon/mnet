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
#include <future>
#include <sstream>
#include <test/framework/Testing.hpp>

namespace apm { namespace test {

class ApplicationTestSuite : public ::test::TestSuite
{
  public:
    ApplicationTestSuite()                            = default;
    ~ApplicationTestSuite() override                  = default;
    ApplicationTestSuite(const ApplicationTestSuite&) = delete;
    ApplicationTestSuite& operator=(const ApplicationTestSuite&) = delete;
    ApplicationTestSuite(ApplicationTestSuite&&)                 = delete;
    ApplicationTestSuite& operator=(ApplicationTestSuite&&) = delete;

  public:
    void start()
    {
        // clang-format off
        thread = std::async(std::launch::async, [this]
        {
            app.run();
        });
        // clang-format on
    }

    void stop() { app.postEvent(Event::makeExitEvent()); }

    void expectNoOutput() { EXPECT_EQ("", tout.str()); }

    void expectOutput(const std::string& output)
    {
        EXPECT_EQ(output, tout.str());
        consume();
    }

    void expectBecomesOutput(const std::string& output)
    {
        expectBecomesEq(output, tout.str());
        consume();
    }

    void expectCsvHeader()
    {
        static constexpr const char* csvHeader =
            "Source, Raw, Modifier, Type, Time\n";

        EXPECT_EQ(csvHeader, tout.str());
        consume();
    }

    void consume()
    {
        tout.str(std::string());
        // This is not thread safe.
        expectNoOutput();
    }

  protected:
    std::stringstream tout;
    Application       app{tout};
    std::future<void> thread;
};

}} // namespace apm::test
