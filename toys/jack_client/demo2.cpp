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

#include "jack_client.h"
#include <math.h>

class Demo2 :
	public JackClient
{
public:
	Demo2() :
		JackClient("demo2")
	{
		open_audio_out_ports(1);
		open_midi_in_ports(1);
		activate();
	}

	~Demo2()
	{
		deactivate();
		close_audio_out_ports();
		close_midi_in_ports();
	}

protected:
	void on_process()
	{
		jack_default_audio_sample_t* out = get_audio_out_samples(0);
		if (out == NULL)
			throw "get_audio_out_samples returned NULL!";

		float F = (2 * M_PI * _frequency) / sample_rate();
		for (int i = 0; i < buffer_size(); ++i)
		{
			out[i] = _amplitude * sin(F * i);
		}
	}

	void on_note_off(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) 
	{
		_frequency = 0.0;
		_amplitude = 0.0;
	}

	void on_note_on(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) 
	{
		_frequency = note_to_frequency(note);
		_amplitude = 1.0;
	}

private:
	float note_to_frequency(jack_midi_data_t note)
	{
		int n = note;
		if (n >= 0 && n <= 119)
			return 440.0 * pow(2.0, (n-69) / 12.0);
		else
			return 0.0;
	}

	float _frequency;
	jack_default_audio_sample_t _amplitude;
};


int 
main(int argc, char **argv)
{
	Demo2* client = new Demo2();

	while (1)
		sleep(1);
}
