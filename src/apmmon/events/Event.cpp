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

#include <events/Event.hpp>

namespace apm {

Event::Event()
    : Event(Source::Ignore)
{
}

Event
Event::makeExitEvent()
{
    return Event(Source::Exit);
}

Event
Event::makeIgnoreEvent()
{
    return Event(Source::Ignore);
}

Event::Event(
    const TimePoint&   _timeOfEvent,
    Key                _key,
    KeyModifier        _keyModifier,
    const std::string& _raw)
    : timeOfEvent(_timeOfEvent)
    , source(Source::Keyboard)
    , raw(_raw)
    , key(_key)
    , keyModifier(_keyModifier)
{
}

bool
Event::isExit() const
{
    return source == Source::Exit;
}

bool
Event::shouldIgnore() const
{
    return source == Source::Ignore;
}

bool
Event::isAction() const
{
    return type == Type::Action;
}

Source
Event::getSource() const
{
    return source;
}

std::string
Event::getRaw() const
{
    return raw;
}

Key
Event::getKey() const
{
    return key;
}

KeyModifier
Event::getKeyModifier() const
{
    return keyModifier;
}

TimePoint
Event::getTimeOfEvent() const
{
    return timeOfEvent;
}

bool
Event::operator<(const TimePoint& time)
{
    return this->timeOfEvent < time;
}

bool
Event::operator>(const TimePoint& time)
{
    return this->timeOfEvent > time;
}

void
Event::setAction()
{
    type = Type::Action;
}

Apm
Event::getInstanceApm() const
{
    return instanceApm;
}

void
Event::setInstanceApm(Apm _instanceApm)
{
    instanceApm = _instanceApm;
}

Event::Event(Source _source)
    : source(_source)
{
}

} // namespace apm
