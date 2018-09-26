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

#include <fstream>
#include <iostream>
#include <linux/InputDeviceCollector.hpp>

namespace apm {

InputDeviceCollector::InputDeviceCollector()
{
    populateInputDevicesRegistery();
}

InputDevices
InputDeviceCollector::collectAllKeyboardsAndMice() const
{
    InputDevices devices;
    for (auto& inputDeviceDescriptor : inputDevicesRegistry)
    {
        if (inputDeviceDescriptor.isKeyboard())
        {
            InputDevice device(inputDeviceDescriptor.getInputDevice());
            if (device.isValid() and device.isKeyboard())
            {
                std::cout << "Monitoring keyboard "
                          << inputDeviceDescriptor.getName() << '\n';
                devices.emplace_back(device.getEpollEvent());
            }
        }
        else if (inputDeviceDescriptor.isMouse())
        {
            InputDevice device(inputDeviceDescriptor.getInputDevice());
            if (device.isValid())
            {
                std::cout << "Monitoring mouse "
                          << inputDeviceDescriptor.getName() << '\n';
                devices.emplace_back(device.getEpollEvent());
            }
        }
    }
    return devices;
}

void
InputDeviceCollector::populateInputDevicesRegistery()
{
    std::ifstream regFile(inputDevicesRegistryPath);

    InputDeviceDescriptor current;
    for (std::string line; std::getline(regFile, line);)
    {
        if (line.empty())
        {
            inputDevicesRegistry.emplace_back(std::move(current));
            current = InputDeviceDescriptor();
        }
        else
        {
            current.parseLine(line);
        }
    }
}

} // namespace apm
