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

#include <algorithm>
#include <iterator>
#include <linux/InputDeviceDescriptor.hpp>
#include <sstream>

namespace apm {

InputDevice
InputDeviceDescriptor::getInputDevice() const
{
    return InputDevice(getDevicePath());
}

void
InputDeviceDescriptor::parseLine(const std::string& line)
{
    static constexpr const int   headerOffset = 3;
    static constexpr const char* delimiter    = "=";
    auto                         delim        = line.find(delimiter);
    if (delim != std::string::npos and delim >= headerOffset)
    {
        auto type  = line[0];
        auto key   = line.substr(headerOffset, delim - headerOffset);
        auto value = line.substr(delim + 1, std::string::npos);
        fields.emplace(key, Field{type, value});
    }
}

bool
InputDeviceDescriptor::isKeyboard() const
{
    static auto kf = getBitSet(keyboardFlags);
    return (parseBitValue(eventsSupported) & kf) == kf;
}

bool
InputDeviceDescriptor::isMouse() const
{
    for (auto& handler : getHandlers())
    {
        // if (handler.starts_with(mouse))
        if (handler.substr(0, std::string(mouse).length()) == mouse)
        {
            return true;
        }
    }
    return false;
}

bool
InputDeviceDescriptor::isKeyboardOrMouse() const
{
    return isKeyboard() or isMouse();
}

std::string
InputDeviceDescriptor::getName() const
{
    return find(name).value;
}

fs::path
InputDeviceDescriptor::getDevicePath() const
{
    for (auto& handler : getHandlers())
    {
        // if (handler.starts_with(event))
        if (handler.substr(0, std::string(event).length()) == event)
        {
            return fs::path(inputDevicesPath) / handler;
        }
    }
    throw std::out_of_range(find(handlers).value);
}

std::vector<std::string>
InputDeviceDescriptor::getHandlers() const
{
    std::istringstream iss(find(handlers).value);
    return std::vector<std::string>{std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>{}};
}

InputDeviceDescriptor::Field
InputDeviceDescriptor::find(const std::string& key) const
{
    auto search = fields.find(key);
    if (search == fields.end())
    {
        throw std::out_of_range(key);
    }
    return search->second;
}

std::bitset<64>
InputDeviceDescriptor::parseBitValue(const std::string& key) const
{
    auto field = find(key);
    if (field.type == typeBitmaps)
    {
        return std::stoul(field.value, nullptr, 16);
    }
    return 0;
}

std::bitset<64>
InputDeviceDescriptor::getBitSet(const std::initializer_list<size_t> flags)
{
    std::bitset<64> ret;
    for (size_t flag : flags)
    {
        ret.set(flag);
    }
    return ret;
}

} // namespace apm
