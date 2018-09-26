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
#include <atomic>
#include <EventQueue.hpp>
#include <observers/EventModifier.hpp>
#include <observers/EventSubject.hpp>

namespace apm {

class ApmCalculator final
    : public EventModifier
    , public EventSubject
{
  public:
    explicit ApmCalculator(
        const std::chrono::milliseconds& _sampleInterval =
            defaultSampleInterval);
    ~ApmCalculator() override = default;

    ApmCalculator(const ApmCalculator&) = delete;
    ApmCalculator& operator=(const ApmCalculator&) = delete;
    ApmCalculator(ApmCalculator&&)                 = delete;
    ApmCalculator& operator=(ApmCalculator&&) = delete;

    Apm getActionsPerMinute() const;

  private:
    void modify(Event event) override;

    void drain(const TimePoint& now);

  private:
    static constexpr std::chrono::milliseconds defaultSampleInterval{60000};
    const std::chrono::milliseconds            sampleInterval;
    const Apm                                  apmRatio;
    EventQueue                                 recentEvents;
    std::atomic<Apm>                           apm;
    Event                                      drainHead;
};

} // namespace apm
