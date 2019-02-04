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

#apmmon

Welcome to apmmon e.g. "Actions per Minute Monitor". This is a cross platform
monitor which hooks into the OS, records actions taken, and processes them.
This is meant to be used for **Real Time Strategy** gaming but it has a variety
of uses.

This is open source so users don't need to be leery about installing a keylogger
on their system. Everyone is free to examine the source code and contributions
are welcome. Please check the contributing section at the bottom of this
document to find some areas in need of work.

##Architecture

The basic architecture of the system consists of an OS specific event monitor,
an unbounded event queue populated by the monitor, and a event pipeline and
multiplexer which drains the event queue.

The event monitor is very specific to the platform being run on and is described
in more detail in the OS specifics section. The keys architecturally though is
this class runs in its own dedicated thread and using the event push APIs
provided by the OS it populates the event queue. The event monitor is the fast
path of the system and its only responsibility is to create an apmmon event and
put it on the queue. As it will often stand between the users inputs and the
applications being run it must not add substantial latency.

The event queue is a thread safe efficient data structure which while unbounded
should be drained on a regular basis. This draining is done by the event
multiplexer which lives in its own thread and implements a pipeline which events
taken from the event queue pass through. The pipeline is also a multiplexer and
thus implements a tree because any number of observers can subscribe at any
point in the pipeline. Each stage has an effect which it does based on the
event. Certain observers are actually event modifiers in that they take a copy
of the event, modifier it, and send it along.

The current pipeline is as follows:

* OS Event Monitor places event on queue
* * Event Multiplexier removes event from queue
* * * APM classifier modifies the event tagging whether it is an 'action'
* * * * APM calculator adds the instance apm for this event
* * * * * Event Serializer serializes the event to standard out using the csv format

There are formatters which take an apmmon event and formats them according to
their definition. The event serializer takes a formatter and output stream and
output the formatted event to the stream.

###OS Event Monitors

####Linux
The linux event monitoring code uses /dev/input and uses a setcap so it doesn't
need to be run as root. It gathers all the devices in /dev/input which are mice
or keyboards and opens a fd to these. This fd is placed on an epoll structure
and a poller waits for events on this epoll fd. This code also implements a exit
handler which waits for any signal and terminates. It has a systemd service
which can be installed to run in the background.

####Windows
The windows event monitoring code makes use of the windows api keyboard and
mouse low level hooks. It registers with these and implements a message loop.
This can be exited with a ctrl c or ctrl break signal.

####Apple
This code is just a stub for now and should be expanded for those interested in
this working on an apple as well.

##Testing
Currently linux and windows extension code is not tested as those come from the
OS. It would be nice to mock the OS calls out so they can be tested as well.

##Contributing
Here are a lists of tasks that need to be done roughly ordered by their
importance. Please consider contributing.

* Build an OBS platform framework.
* Support building on windows.
* Determine packaging on each operating system.
* Finish event classification.
* Calculate apm with timer.
* Make signals thread safe.`
