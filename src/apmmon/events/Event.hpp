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

#include <Apm.hpp>
#include <events/Key.hpp>
#include <events/KeyModifier.hpp>
#include <events/Source.hpp>
#include <events/Type.hpp>
#include <Time.hpp>

namespace apm {

class Event final
{
  public:
    Event();
    static Event makeExitEvent();
    static Event makeIgnoreEvent();

    Event(
        const TimePoint&   _timeOfEvent,
        Key                _key,
        KeyModifier        _keyModifier,
        const std::string& _raw);
    ~Event()            = default;
    Event(const Event&) = default;
    Event& operator=(const Event&) = default;
    Event(Event&&)                 = default;
    Event& operator=(Event&&) = default;

  public:
    bool isExit() const;
    bool shouldIgnore() const;
    bool isAction() const;

    Source      getSource() const;
    std::string getRaw() const;
    Key         getKey() const;
    KeyModifier getKeyModifier() const;
    TimePoint   getTimeOfEvent() const;
    Apm         getInstanceApm() const;

    void setAction();
    void setInstanceApm(Apm _instanceApm);

    bool operator<(const TimePoint& time);
    bool operator>(const TimePoint& time);

  private:
    explicit Event(Source _source);

  private:
    TimePoint   timeOfEvent;
    Type        type{Type::Unknown};
    Source      source;
    std::string raw;
    Key         key;
    KeyModifier keyModifier;
    Apm         instanceApm{0};
};

} // namespace apm
