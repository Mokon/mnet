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

#include <formatters/CsvFormatter.hpp>
#include <sstream>
#include <Time.hpp>

namespace apm {

CsvFormatter::CsvFormatter(const std::string& _seperator)
    : seperator(_seperator)
{
}

std::string
CsvFormatter::initialize() const
{
    std::stringstream ss;
    ss << "Source" << seperator << "Raw" << seperator << "Modifier" << seperator
       << "Type" << seperator << "Time" << '\n';
    return ss.str();
}

std::string
CsvFormatter::format(const Event& event) const
{
    std::stringstream ss;
    ss << SourceTraits::toString(event.getSource()) << seperator
       << event.getRaw();

    switch (event.getSource())
    {
        case Source::Unknown:
        case Source::Ignore:
        case Source::Exit:
        case Source::Mouse:
            ss << seperator << seperator;
            break;
        case Source::Keyboard:
            ss << seperator << KeyTraits::toString(event.getKey()) << seperator
               << KeyModifierTraits::toString(event.getKeyModifier());
            break;
    }
    ss << seperator << event.getTimeOfEvent() << seperator
       << event.getInstanceApm() << '\n';
    return ss.str();
}

std::string
CsvFormatter::finalize() const
{
    return "";
}

} // namespace apm
