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

#include <Time.hpp>

namespace apm { namespace Time {

TimePoint
now()
{
    return Clock::now();
}

TimePoint
from(struct timeval tv)
{
    return TimePoint{std::chrono::seconds{tv.tv_sec} +
                     std::chrono::microseconds{tv.tv_usec}};
}

std::string
toString(const apm::TimePoint& time)
{
    std::stringstream stream;
    stream << time;
    return stream.str();
}

}} // namespace apm::Time

std::ostream&
operator<<(std::ostream& stream, const apm::TimePoint& time)
{
    std::time_t epoch = std::chrono::duration_cast<std::chrono::seconds>(
                            time.time_since_epoch())
                            .count();
    return stream << std::put_time(std::localtime(&epoch), "%F %T");
}
