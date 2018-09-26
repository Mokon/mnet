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

project (mnet CXX)
message ("${PROJECT_NAME}/${OS}/${CMAKE_HOST_SYSTEM_PROCESSOR}")

cmake_minimum_required(VERSION 3.11.2)
set(CMAKE_128_COLOR true)

set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 0)

include_directories(${PROJECT_SOURCE_DIR}/src/lib)

include(cmake/apmmon.cmake)
include(cmake/${OS}.cmake)

if(CMAKE_CXX_COMPILER MATCHES ".*clang.*")
  include(cmake/clang.cmake)
elseif(CMAKE_CXX_COMPILER MATCHES ".*(gcc|g\\+\\+).*")
  include(cmake/gcc.cmake)
endif()

include(cmake/${CMAKE_BUILD_TYPE}.cmake)

list(APPEND CMAKE_CXX_FLAGS "${CMAKE_CXX_WARNINGS}")
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
