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

#include "organ_voice.h"
#include "../../sdk/common/exception.h"
#include "../../sdk/common/jack_client.h"
#include "../../sdk/common/utility.h"

class SimpleOrgan :
	public nuclear::jack_client
{
public:
	SimpleOrgan() :
		nuclear::jack_client("organ")
	{
		for (int i = 0; i < 8; ++i)
		{
			organ_voice* v = new organ_voice();
			v->init(sample_rate());
			_voices.push_back(v);
		}

		open_audio_out_ports(2);
		open_midi_in_ports(1);
		activate();
	}

	~SimpleOrgan()
	{
		deactivate();
		close_audio_out_ports();
		close_midi_in_ports();
	}

protected:
	void on_process()
	{
		jack_default_audio_sample_t* out_left = get_audio_out_samples(0);
		jack_default_audio_sample_t* out_right = get_audio_out_samples(1);
		if (out_left == NULL || out_right == NULL)
			throw nuclear::Exception("get_audio_out_samples returned NULL!");

		nuclear::init_buffer<jack_default_audio_sample_t>(buffer_size(), out_left);
		nuclear::init_buffer<jack_default_audio_sample_t>(buffer_size(), out_right);
		jack_default_audio_sample_t** output = nuclear::allocate_buffers<jack_default_audio_sample_t>(2, buffer_size());
		for (std::vector<nuclear::voice*>::iterator i = _voices.begin(); i != _voices.end(); ++i)
		{
			(*i)->compute(buffer_size(), NULL, output);
			nuclear::mix_buffer<jack_default_audio_sample_t>(buffer_size(), output[0], out_left);
			nuclear::mix_buffer<jack_default_audio_sample_t>(buffer_size(), output[1], out_right);
		}
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
	SimpleOrgan client;

	while (1)
		sleep(1);
}
