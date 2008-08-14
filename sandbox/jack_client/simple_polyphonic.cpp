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
#include "../../sdk/common/exception.h"
#include "../../sdk/common/jack_client.h"
#include "../../sdk/common/utility.h"

class SimplePolyphonic :
	public nuclear::jack_client
{
public:
	SimplePolyphonic() :
		nuclear::jack_client("polysynth")
	{
		for (int i = 0; i < 8; ++i)
		{
			osci_voice* v = new osci_voice();
			v->init(sample_rate());
			_voices.push_back(v);
		}

		open_audio_out_ports(1);
		open_midi_in_ports(1);
		activate();
	}

	~SimplePolyphonic()
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
			throw nuclear::Exception("get_audio_out_samples returned NULL!");

		nuclear::init_buffer<jack_default_audio_sample_t>(buffer_size(), out);
		
		jack_default_audio_sample_t** output = nuclear::allocate_buffers<jack_default_audio_sample_t>(1, buffer_size());
		for (std::vector<nuclear::voice*>::iterator i = _voices.begin(); i != _voices.end(); ++i)
		{
			if ((*i)->note_playing() == 0) continue;
			(*i)->compute(buffer_size(), NULL, output);
			nuclear::mix_buffer<jack_default_audio_sample_t>(buffer_size(), output[0], out);
		}

		jack_default_audio_sample_t scale = 0.5;
		nuclear::scale_buffer<jack_default_audio_sample_t>(buffer_size(), scale, out);
		nuclear::free_buffers<jack_default_audio_sample_t>(output);
	}

	void on_note_off(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) 
	{
		for (std::vector<nuclear::voice*>::iterator i = _voices.begin(); i != _voices.end(); ++i)
			if ((*i)->note_playing() == note)
				(*i)->stop_note();
	}

	void on_note_on(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) 
	{
		for (std::vector<nuclear::voice*>::iterator i = _voices.begin(); i != _voices.end(); ++i)
		{
			if ((*i)->note_playing() == 0)
			{
				(*i)->play_note(note, velocity);
				break;
			}
		}
	}

private:
	std::vector<nuclear::voice*> _voices;
};


int 
main(int argc, char **argv)
{
	SimplePolyphonic client;

	while (1)
		sleep(1);
}
