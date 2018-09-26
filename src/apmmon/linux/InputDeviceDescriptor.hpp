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

#include <bitset>
#include <linux/InputDevice.hpp>
#include <map>
#include <string>

namespace apm {

class InputDeviceDescriptor final
{
  private:
    struct Field
    {
        char        type;
        std::string value;
    };

  public:
    InputDeviceDescriptor()                             = default;
    ~InputDeviceDescriptor()                            = default;
    InputDeviceDescriptor(const InputDeviceDescriptor&) = delete;
    InputDeviceDescriptor& operator=(const InputDeviceDescriptor&) = delete;
    InputDeviceDescriptor(InputDeviceDescriptor&&)                 = default;
    InputDeviceDescriptor& operator=(InputDeviceDescriptor&&) = default;

  public:
    InputDevice getInputDevice() const;

    void parseLine(const std::string& line);

    bool isKeyboard() const;
    bool isMouse() const;
    bool isKeyboardOrMouse() const;

    std::string getName() const;

    fs::path getDevicePath() const;

  private:
    std::vector<std::string> getHandlers() const;

    Field find(const std::string& key) const;

    std::bitset<64> parseBitValue(const std::string& key) const;

    static constexpr const std::initializer_list<size_t> keyboardFlags =
        {EV_SYN, EV_KEY, EV_MSC, EV_LED, EV_REP};

    static std::bitset<64> getBitSet(const std::initializer_list<size_t> flags);

  private:
    // Keys
    static constexpr const char* idBus           = "Bus";
    static constexpr const char* idVendor        = "Vendor";
    static constexpr const char* idProduct       = "Product";
    static constexpr const char* idVersion       = "Version";
    static constexpr const char* name            = "Name";
    static constexpr const char* phys            = "Phys";
    static constexpr const char* sysfs           = "Sysfs";
    static constexpr const char* uniq            = "Uniq";
    static constexpr const char* handlers        = "Handlers";
    static constexpr const char* properties      = "PROP";
    static constexpr const char* eventsSupported = "EV";
    static constexpr const char* keysSupported   = "KEY";
    static constexpr const char* misc            = "MSC";
    static constexpr const char* leds            = "LED";

    // Types
    static constexpr const char typeId       = 'I';
    static constexpr const char typeName     = 'N';
    static constexpr const char typePhys     = 'P';
    static constexpr const char typeSysfs    = 'S';
    static constexpr const char typeUniq     = 'U';
    static constexpr const char typeHandlers = 'H';
    static constexpr const char typeBitmaps  = 'B';

    static constexpr const char* inputDevicesPath = "/dev/input/";

    static constexpr const char* event = "event";
    static constexpr const char* mouse = "mouse";

  private:
    std::map<std::string, Field> fields;
};

} // namespace apm
