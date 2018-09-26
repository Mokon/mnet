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

#include <observers/ApmCalculator.hpp>
#include <Time.hpp>

namespace apm {

ApmCalculator::ApmCalculator(const std::chrono::milliseconds& _sampleInterval)
    : sampleInterval(_sampleInterval)
    , apmRatio(
          static_cast<Apm>(defaultSampleInterval.count()) /
          static_cast<Apm>(sampleInterval.count()))
{
}

Apm
ApmCalculator::getActionsPerMinute() const
{
    return apm.load();
}

void
ApmCalculator::modify(Event event)
{
    recentEvents.emplace(event);
    drain(Time::now());
    event.setInstanceApm(getActionsPerMinute());

    mux(event);
}

void
ApmCalculator::drain(const TimePoint& now)
{
    while (not drainHead.shouldIgnore() or recentEvents.try_pop(drainHead))
    {
        // TODO handle future events
        if (drainHead > now - defaultSampleInterval)
        {
            break;
        }
        drainHead = Event();
    }
    auto numRecentEvents =
        recentEvents.size() + (drainHead.shouldIgnore() ? 0 : 1);
    apm.store(static_cast<Apm>(numRecentEvents) * apmRatio);
}

} // namespace apm
