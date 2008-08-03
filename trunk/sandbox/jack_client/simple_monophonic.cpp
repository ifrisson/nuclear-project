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

#include "osci_voice.h"
#include "../../sdk/common/jack_client.h"

class SimpleMonophonic :
	public nuclear::jack_client
{
public:
	SimpleMonophonic() :
		nuclear::jack_client("monosynth")
	{
		osci_voice* v = new osci_voice();
		v->init(sample_rate());
		_voice = v;

		open_audio_out_ports(1);
		open_midi_in_ports(1);
		activate();
	}

	~SimpleMonophonic()
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

		jack_default_audio_sample_t* output[] = { out };
		_voice->compute(buffer_size(), NULL, output);
	}

	void on_note_off(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) 
	{
		_voice->stop_note();
	}

	void on_note_on(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) 
	{
		_voice->play_note(note, velocity);
	}

private:
	nuclear::voice* _voice;
};


int 
main(int argc, char **argv)
{
	SimpleMonophonic client;

	while (1)
		sleep(1);
}
