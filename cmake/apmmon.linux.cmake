# Copyright (c) 2018 David 'Mokon' Bond <mokon@mokon.net>
# Author: David 'Mokon' Bond <mokon@mokon.net>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

add_custom_target(generateInputEventCodes ALL
  ${PROJECT_SOURCE_DIR}/tools/codegen/generateInputEventCodes)

add_custom_target(setcap ALL
  sudo setcap CAP_DAC_READ_SEARCH=ep ${PROJECT_BINARY_DIR}/apmmon)
add_dependencies(setcap apmmon)

target_link_libraries(apmmon
  stdc++fs
  tbb
  pthread
  gtest
  cap)
add_dependencies(apmmon generateInputEventCodes)

target_sources(apmmon PUBLIC
  ${PROJECT_SOURCE_DIR}/src/apmmon/linux/ExitHandler.cpp
  ${PROJECT_SOURCE_DIR}/src/apmmon/linux/InputDeviceCollector.cpp
  ${PROJECT_SOURCE_DIR}/src/apmmon/linux/InputDeviceDescriptor.cpp
  ${PROJECT_SOURCE_DIR}/src/apmmon/linux/InputDevice.cpp
  ${PROJECT_SOURCE_DIR}/src/apmmon/linux/InputEventClassifier.cpp
  ${PROJECT_SOURCE_DIR}/src/apmmon/linux/InputEventReader.cpp
  ${PROJECT_SOURCE_DIR}/src/apmmon/linux/Poller.cpp
  ${PROJECT_SOURCE_DIR}/src/apmmon/linux/PrivilegeHandler.cpp
  ${PROJECT_SOURCE_DIR}/src/apmmon/linux/ScopedPrivilege.cpp)
