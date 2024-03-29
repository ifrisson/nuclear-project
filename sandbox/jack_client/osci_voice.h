#ifndef _OSCI_VOICE_H
#define _OSCI_VOICE_H

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
#include "osci.dsp.in"

class osci_voice :
	public nuclear::voice
{
public:
	osci_voice() :
		nuclear::voice(),
		_note(0),
		_interface(new nuclear::faust::paramui())
	{
	}

	static port_t expected_audio_inputs() { return 0; }
	static port_t expected_audio_outputs() { return 1; }

	void init(nuclear::uint32_t srate)
	{
		_dsp.init(srate);
		_dsp.buildUserInterface(_interface);

		try
		{
			_interface->set_option("/oscillator/freq", 0.000000f);
			_interface->set_option("/oscillator/volume", -96.000000f);
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
			_interface->set_option("/oscillator/freq", note_to_frequency(_note));
			_interface->set_option("/oscillator/volume", velocity_to_amplitude(velocity));
		}
		catch (nuclear::Exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void stop_note()
	{
		_timestamp = 0;
		_note = 0;

		try
		{
			_interface->set_option("/oscillator/freq", 0.000000f);
			_interface->set_option("/oscillator/volume", -96.000000f);
		}
		catch (nuclear::Exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void kill_note()
	{
		// Same as stop_note() since we don't have any release envelope
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
		int n = note;
		if (n >= 0 && n <= 119)
			return 440.0 * pow(2.0, (n-69) / 12.0);
		else
			return 0.0;
	}

	float velocity_to_amplitude(nuclear::velocity_t velocity)
	{
		return -96.000000f + 96.000000f / 127 * velocity;
	}

	nuclear::uint8_t _note;
	nuclear::double_t _timestamp;

	mydsp _dsp;
	nuclear::faust::paramui* _interface;
};

#endif // !_OSCI_VOICE_H
