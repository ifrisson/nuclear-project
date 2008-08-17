// Simple Organ
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
#include "../../sdk/synth.h"
#include "../../sdk/synth_descriptor.h"
#include "../../sdk/common/dssi_plugin.h"
#include "../../sdk/common/utility.h"

class organ_synth :
	public nuclear::synth
{
public:
	organ_synth()
	{
	}

	~organ_synth()
	{
	}

	void note_off(nuclear::note_t note, nuclear::velocity_t velocity) 
	{
		for (std::vector<nuclear::voice*>::iterator i = _voices.begin(); i != _voices.end(); ++i)
			if ((*i)->note_playing() == note)
				(*i)->stop_note();
	}

	void note_on(nuclear::note_t note, nuclear::velocity_t velocity) 
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

	void aftertouch(nuclear::note_t note, nuclear::velocity_t velocity) {}
	void channel_pressure(nuclear::velocity_t pressure) {}
	void pitch_bend(nuclear::velocity_t value) {}
	void program_change(nuclear::uint32_t bank, nuclear::uint32_t program) {}
	void configure(nuclear::string_t key, nuclear::string_t value) {}

        void init(nuclear::uint32_t srate) 
	{
		for (int i = 0; i < 8; ++i)
		{
			organ_voice* v = new organ_voice();
			v->init(srate);
			_voices.push_back(v);
		}
	}

        void run(nuclear::nframes_t nframes)
	{
		nuclear::init_buffer<sample_t>(nframes, p(0));
		nuclear::init_buffer<sample_t>(nframes, p(1));
		sample_t** out = nuclear::allocate_buffers<sample_t>(2, nframes);
		for (std::vector<nuclear::voice*>::iterator i = _voices.begin(); i != _voices.end(); ++i)
		{
			(*i)->process_audio(nframes, NULL, out);
			nuclear::mix_buffer<sample_t>(nframes, out[0], p(0));
			nuclear::mix_buffer<sample_t>(nframes, out[1], p(1));
		}
		nuclear::free_buffers<sample_t>(out);
	}

	float_t* p(nuclear::port_t index)
	{
		return _ports[index];
	}

	void connect_port(nuclear::port_t index, nuclear::float_t* data)
	{
		_ports[index] = data;
	}

	static void build_descriptor(nuclear::synth_descriptor* descriptor)
	{
		descriptor->set_label("simple_organ");
		descriptor->set_name("Simple Organ");
		descriptor->set_maker("Anders Dahnielson <anders@dahnielson.com>");
		descriptor->set_copyright("2008, Anders Dahnielson");
		descriptor->add_output("Left");
		descriptor->add_output("Right");
	}

private:
	std::vector<nuclear::voice*> _voices;
	nuclear::float_t* _ports[2];
};

NUCLEAR_REGISTER_DSSI(organ_synth)
