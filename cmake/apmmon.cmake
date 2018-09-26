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

include_directories(${PROJECT_BINARY_DIR}/src/apmmon)
include_directories(${PROJECT_SOURCE_DIR}/src/apmmon)

add_executable(apmmon
  src/apmmon/Application.cpp
  src/apmmon/classifiers/ApmClassifier.cpp
  src/apmmon/EventMonitor.cpp
  src/apmmon/EventMultiplexer.cpp
  src/apmmon/events/Event.cpp
  src/apmmon/formatters/CsvFormatter.cpp
  src/apmmon/observers/ApmCalculator.cpp
  src/apmmon/observers/EventModifier.cpp
  src/apmmon/observers/EventSerializer.cpp
  src/apmmon/observers/EventSubject.cpp
  src/apmmon/${OS}/main.cpp
  src/apmmon/${OS}/EventMonitor.cpp
  src/apmmon/Time.cpp)

add_custom_target(generateEnums ALL
  ${PROJECT_SOURCE_DIR}/tools/codegen/generateEnum src/apmmon/events/Key.enum
  COMMAND ${PROJECT_SOURCE_DIR}/tools/codegen/generateEnum src/apmmon/events/KeyModifier.enum
  COMMAND ${PROJECT_SOURCE_DIR}/tools/codegen/generateEnum src/apmmon/events/Source.enum
  COMMAND ${PROJECT_SOURCE_DIR}/tools/codegen/generateEnum src/apmmon/events/Type.enum
)
add_dependencies(apmmon generateEnums)

include(cmake/apmmon.${OS}.cmake)
