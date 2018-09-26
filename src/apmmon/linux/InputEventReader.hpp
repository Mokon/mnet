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

#include <linux/input.h>

namespace apm {

static constexpr const int maxInputEventsPerCycle = 10;

class InputEventReader final
{
  public:
    InputEventReader()                        = default;
    ~InputEventReader()                       = default;
    InputEventReader(const InputEventReader&) = delete;
    InputEventReader& operator=(const InputEventReader&) = delete;
    InputEventReader(InputEventReader&&)                 = delete;
    InputEventReader& operator=(InputEventReader&&) = delete;

  public:
    void read(int fd);

    bool hasMore() const;

    const struct input_event& next();

  private:
    struct input_event inputEvents[maxInputEventsPerCycle];
    size_t             currentEventIndex;
    size_t             currentEventMax;
};

} // namespace apm
