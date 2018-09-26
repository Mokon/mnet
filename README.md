> Copyright (c) 2018 David 'Mokon' Bond <mokon@mokon.net>
> Author: David 'Mokon' Bond <mokon@mokon.net>
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of
> this software and associated documentation files (the "Software"), to deal in
> the Software without restriction, including without limitation the rights to
> use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
> the Software, and to permit persons to whom the Software is furnished to do so,
> subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all
> copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
> FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
> COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
> IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
> CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#mnet

Welcome to mnet e.g. "Mokon Net's Open Source Library".

##Building & Installation

mnet uses cmake as its build system and also has a top level Makefile to help
the developer get started. After cloning the repository on a linux distro
executing ./tools/deploy will install the packages required to build mnet.
While mnet is cross platform the deploy script will not work on a windows
system. This script will either install the packages required or build them.

All temporary and generated files from the build process are places in the build
folder. Nothing valuable should be placed here as that directory is regularly
destroyed.

There are a number of targets including:

* `build/release/linux`
* `build/release/windows`
* `build/release/test`
* `build/debug/linux`
* `build/debug/windows`
* `build/debug/test`

Folders to these are generated and inside them the application is created.

`make install` will install the program.

`make test` will run all the unit test for the program.

`make clean` will delete the build directory.

##Testing
mnet uses gtest for unit tests.

##Contributing
Please consider contributing. Individual applications will have sections on what
needs to be done.
