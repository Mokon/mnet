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

all: clean build/release/linux
build/release: build/release/linux build/release/windows build/release/test
build/debug: build/debug/linux build/debug/windows build/debug/test
build/release/% build/debug/%:
	mkdir -p $@
	cd $@ ; cmake -DCMAKE_BUILD_TYPE=$(subst build/,,$(@D)) -DOS=$(@F) ../../../
	cd $@ ; make
install:
	install build/release/release/apmmon /usr/bin
	install -m 644 packaging/apmmon.service /etc/systemd/system/
test: build/release/test
	./tools/test
clean:
	/bin/rm -rf build/
