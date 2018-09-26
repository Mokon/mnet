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

#include <events/Event.hpp>
#include <unordered_map>

namespace apm {

class InputEventClassifier final
{
  public:
    InputEventClassifier();
    ~InputEventClassifier()                           = default;
    InputEventClassifier(const InputEventClassifier&) = delete;
    InputEventClassifier& operator=(const InputEventClassifier&) = delete;
    InputEventClassifier(InputEventClassifier&&)                 = delete;
    InputEventClassifier& operator=(InputEventClassifier&&) = delete;

    using Code  = uint16_t;
    using Value = int32_t;

  public:
    void populateInputEventCodes();

    Event get(const struct input_event& inputEvent);

    static KeyModifier getKeyModifier(Value value);

    static Key getKey(Code key);

  private:
    std::unordered_map<Code, std::string> inputEventCodes;
};

} // namespace apm
