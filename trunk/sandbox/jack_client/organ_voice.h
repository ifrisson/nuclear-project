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

#include "../../sdk/voice.h"
#include "../../sdk/faust/dsp.h"
#include "../../sdk/faust/common/paramui.h"
#include <math.h>
#include <ctime>

using nuclear::nframes_t;
using nuclear::sample_t;
using nuclear::port_t;

using nuclear::faust::dsp;
using nuclear::faust::UI;

// Include the generated file
#include "organ.dsp.in"

class organ_voice :
	public nuclear::voice
{
public:
	organ_voice() :
		nuclear::voice(),
		_note(0),
		_interface(new nuclear::faust::paramui())
	{
	}

	static port_t expected_audio_inputs() { return 0; }
	static port_t expected_audio_outputs() { return 2; }

	void init(nuclear::uint32_t srate)
	{
		_dsp.init(srate);
		_dsp.buildUserInterface(_interface);

		try
		{
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
		catch (nuclear::Exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void process_audio(nframes_t nframes, sample_t** inputs, sample_t** outputs)
	{
		_dsp.compute(nframes, inputs, outputs);
	}

	void play_note(nuclear::note_t note, nuclear::velocity_t velocity)
	{
		_timestamp = std::clock();
		_note = note;

		try
		{
			_interface->set_option("/faust/freq", note_to_frequency(_note));
			_interface->set_option("/faust/gain", velocity);
			_interface->set_option("/faust/gate", 1.0f);
		}
		catch (nuclear::Exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void stop_note()
	{
		//todo: How to deal with the release envelope?
		_timestamp = 0;
		_note = 0;

		try
		{
			_interface->set_option("/faust/gate", 0.0f);
		}
		catch (nuclear::Exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void kill_note()
	{
		stop_note();
	}

	nuclear::note_t note_playing()
	{
		return _note;
	}

	nuclear::double_t note_timestamp()
	{
		return _timestamp;
	}

private:
	float note_to_frequency(nuclear::note_t note)
	{
		if (note >= 0 && note <= 127)
			return 440.0 * pow(2.0, (note-69) / 12.0);
		else
			return 0.0;
	}

	nuclear::note_t _note;
	nuclear::double_t _timestamp;

	mydsp _dsp;
	nuclear::faust::paramui* _interface;
};

#endif // !_ORGAN_VOICE_H
