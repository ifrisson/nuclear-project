#ifndef PERFORMER_COMMANDS_H
#define PERFORMER_COMMANDS_H

// Vivace Performer
// Copyright (c) 2008, Anders Dahnielson
//
// Contact: anders@dahnielson.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "jack_client.h"

class Command
{
public:
	virtual void execute() = 0;
	virtual ~Command() {}
};

class NoteOnCommand :
	public Command
{
public:
	NoteOnCommand(JackClient* receiver, const int port, const int channel, const int note, const int velocity) : 
		receiver(receiver),
		port(port),
		channel(channel),
		note(note),
		velocity(velocity)
	{
	}

	void execute()
	{
		receiver->note_on(port, channel, note, velocity);
	}

private:
        JackClient* receiver;
	const int port;
	const int channel;
	const int note;
	const int velocity;
};

class NoteOffCommand :
	public Command
{
public:
	NoteOffCommand(JackClient* receiver, const int port, const int channel, const int note, const int velocity) : 
		receiver(receiver),
		port(port),
		channel(channel),
		note(note),
		velocity(velocity)
	{
	}

	void execute()
	{
		receiver->note_off(port, channel, note, velocity);
	}

private:
        JackClient* receiver;
	const int port;
	const int channel;
	const int note;
	const int velocity;
};

class AftertouchCommand :
	public Command
{
public:
	AftertouchCommand(JackClient* receiver, const int port, const int channel, const int note, const int velocity) : 
		receiver(receiver),
		port(port),
		channel(channel),
		note(note),
		velocity(velocity)
	{
	}

	void execute()
	{
		receiver->aftertouch(port, channel, note, velocity);
	}

private:
        JackClient* receiver;
	const int port;
	const int channel;
	const int note;
	const int velocity;
};

class ContinousControllerCommand :
	public Command
{
public:
	ContinousControllerCommand(JackClient* receiver, const int port, const int channel, const int controller, const int value) : 
		receiver(receiver),
		port(port),
		channel(channel),
		controller(controller),
		value(value)
	{
	}

	void execute()
	{
		receiver->continous_controller(port, channel, controller, value);
	}

private:
        JackClient* receiver;
	const int port;
	const int channel;
	const int controller;
	const int value;
};

class PatchChangeCommand :
	public Command
{
public:
	PatchChangeCommand(JackClient* receiver, const int port, const int channel, const int program) : 
		receiver(receiver),
		port(port),
		channel(channel),
		program(program)
	{
	}

	void execute()
	{
		receiver->patch_change(port, channel, program);
	}

private:
        JackClient* receiver;
	const int port;
	const int channel;
	const int program;
};

class ChannelPressureCommand :
	public Command
{
public:
	ChannelPressureCommand(JackClient* receiver, const int port, const int channel, const int pressure) : 
		receiver(receiver),
		port(port),
		channel(channel),
		pressure(pressure)
	{
	}

	void execute()
	{
		receiver->channel_pressure(port, channel, pressure);
	}

private:
        JackClient* receiver;
	const int port;
	const int channel;
	const int pressure;
};

class PitchBendCommand :
	public Command
{
public:
	PitchBendCommand(JackClient* receiver, const int port, const int channel, const int value_lo, const int value_hi) : 
		receiver(receiver),
		port(port),
		channel(channel),
		value_lo(value_lo),
		value_hi(value_hi)
	{
	}

	void execute()
	{
		receiver->pitch_bend(port, channel, value_lo, value_hi);
	}

private:
        JackClient* receiver;
	const int port;
	const int channel;
	const int value_lo;
	const int value_hi;
};

#endif // !PERFORMER_COMMANDS_H
