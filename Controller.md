# Introduction #

The SDK use [OpenSound Control](http://opensoundcontrol.org) for all messages (including communication with the front-end/GUI). The neat thing about OSC is that we can have every single node and voice addressable and controlled from the outside, like changing setting of individual nodes in specific voices or switching voices on and off. For normal operation all those details are handled by algorithms provided by the SDK, but having them addressable makes it possible e.g. to implement more esoteric controls.

Each tier will have a controller object (or at least be associated with one) responsible for receiving messages implementing so called OSC methods that either act directly on the tier (be it the engine, voice or node) or send a new message to the next tier. E.g the engines controller receive a _note on_ message, the engine controller (where the voice stealing algorithm is implemented) send a new message addressed to one of the unused voices to set the pitch, velocity and enable it; the particular voices controller receive the message from the engine and sends several messages to its nodes, one to the oscillator with the pitch frequency and another to the amplifier with a new volume value derived from the velocity. Basically the standard behavior is a message handling hierarchy using OSC for communication, but nothing stops you from addressing each node controller individually from outside the engine if you want to do that.

Inspired by the [OSC 'SYN' namespace](http://stud3.tuwien.ac.at/~e0725639/OSC-SYN.txt) proposal the engine ignore the notion of MIDI altogether (interfacing with MIDI devices is up to the host to handle).

Obviously any "MIDI scripting" can be done in a controller that implements a suitable script interpreter.