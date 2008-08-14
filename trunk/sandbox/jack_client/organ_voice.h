#ifndef _ORGAN_VOICE_H
#define _ORGAN_VOICE_H

// Simple JACK Client Demo
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

#include "../../sdk/dsp.h"
#include "../../sdk/voice.h"
#include "../../sdk/common/paramui.h"
#include <math.h>
#include <ctime>

using nuclear::dsp;
using nuclear::UI;

// Include the generated file
#include "organ.dsp.in"

class organ_voice :
	public nuclear::voice
{
public:
	organ_voice() :
		nuclear::voice(),
		_note(0),
		_interface(new nuclear::paramui())
	{
	}

	int getNumInputs() { return 0; }
	int getNumOutputs() { return 1;	}

	void buildUserInterface(UI* interface)
	{
		_dsp.buildUserInterface(interface);
	}

	void init(int srate)
	{
		_dsp.init(srate);
		_dsp.buildUserInterface(_interface);

		_interface->set_option("/faust/1-adsr/attack", 1.000000e-02f);
		_interface->set_option("/faust/1-adsr/decay", 0.300000f);
		_interface->set_option("/faust/1-adsr/release", 0.200000f);
		_interface->set_option("/faust/1-adsr/sustain", 0.500000f);

		_interface->set_option("/faust/2-master/pan", 0.500000f);
		_interface->set_option("/faust/2-master/vol", 0.300000f);

		_interface->set_option("/faust/freq", 440.000000f);
		_interface->set_option("/faust/gain", 0.300000f);
		_interface->set_option("/faust/gate", 0.0f);
	}

	void compute(int nframes, float** inputs, float** outputs)
	{
		_dsp.compute(nframes, inputs, outputs);
	}

	void play_note(nuclear::uint8_t note, nuclear::uint8_t velocity)
	{
		_timestamp = std::clock();
		_note = note;
		_interface->set_option("/faust/freq", note_to_frequency(_note));
		_interface->set_option("/faust/gain", velocity_to_amplitude(velocity));
		_interface->set_option("/faust/gate", 1.0f);
	}

	void stop_note()
	{
		//fixme: How to deal with the release envelope?
		_timestamp = 0;
		_note = 0;
		_interface->set_option("/faust/gate", 0.0f);
	}

	void kill_note()
	{
		stop_note();
	}

	nuclear::uint8_t note_playing()
	{
		return _note;
	}

	nuclear::double_t note_timestamp()
	{
		return _timestamp;
	}

private:
	float note_to_frequency(nuclear::uint8_t note)
	{
		int n = note;
		if (n >= 0 && n <= 119)
			return 440.0 * pow(2.0, (n-69) / 12.0);
		else
			return 0.0;
	}

	float velocity_to_amplitude(nuclear::uint8_t velocity)
	{
		int v = velocity;
		return -96.000000f + 96.000000f / 127 * v;
	}

	nuclear::uint8_t _note;
	nuclear::double_t _timestamp;

	mydsp _dsp;
	nuclear::paramui* _interface;
};

#endif // !_ORGAN_VOICE_H