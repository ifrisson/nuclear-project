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

#include "../../sdk/jack_client.h"
#include "../../sdk/dsp.h"
#include "../../sdk/paramui.h"
#include "../../sdk/voice.h"
#include <math.h>

using nuclear::dsp;
using nuclear::UI;

// Include the generated file
#include "osci.dsp.in"

class Voice :
	public nuclear::voice
{
public:
	Voice() :
		nuclear::voice(),
		_note(0),
		_interface(new nuclear::paramui())
	{
	}

	int getNumInputs() { return 0; }
	int getNumOutputs() { return 1;	}
	void buildUserInterface(UI* interface) {}

	void init(int srate)
	{
		_dsp.init(srate);
		_dsp.buildUserInterface(_interface);
		_interface->set_option("oscillator/freq", 0.000000f);
		_interface->set_option("oscillator/volume", -96.000000f);
	}

	void compute(int nframes, float** inputs, float** outputs)
	{
		_dsp.compute(nframes, inputs, outputs);
	}

	void play_note(jack_midi_data_t note)
	{
		_note = note;
		_interface->set_option("oscillator/freq", note_to_frequency(_note));
		_interface->set_option("oscillator/volume", 0.000000f);
	}

	void stop_note()
	{
		_note = 0;
		_interface->set_option("oscillator/freq", 0.000000f);
		_interface->set_option("oscillator/volume", -96.000000f);
	}

	void kill_note()
	{
		// Same as stop_note() since we don't have any release envelope
		stop_note();
	}

	jack_midi_data_t note_playing()
	{
		return _note;
	}

private:
	jack_default_audio_sample_t note_to_frequency(jack_midi_data_t note)
	{
		int n = note;
		if (n >= 0 && n <= 119)
			return 440.0 * pow(2.0, (n-69) / 12.0);
		else
			return 0.0;
	}

	jack_midi_data_t _note;

	mydsp _dsp;
	nuclear::paramui* _interface;
};

class Demo4 :
	public nuclear::jack_client
{
public:
	Demo4() :
		nuclear::jack_client("demo4")
	{
		for (int i = 0; i < 8; ++i)
		{
			Voice* v = new Voice();
			v->init(sample_rate());
			_voices.push_back(v);
		}

		open_audio_out_ports(1);
		open_midi_in_ports(1);
		activate();
	}

	~Demo4()
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

		int nactive = 0;
		jack_default_audio_sample_t* voice_buffer = new jack_default_audio_sample_t[buffer_size()];
		for (std::vector<Voice*>::iterator i = _voices.begin(); i != _voices.end(); ++i)
		{
			if ((*i)->note_playing() == 0) continue;
			else nactive++;

			jack_default_audio_sample_t* output[] = { voice_buffer };
			(*i)->compute(buffer_size(), NULL, output);
			jack_nframes_t n = buffer_size();
			jack_default_audio_sample_t* src = voice_buffer;
			jack_default_audio_sample_t* dst = out;
			while (n--) *dst++ += *src++;
		}

		jack_nframes_t n = buffer_size();
		jack_default_audio_sample_t* dst = out;
		jack_default_audio_sample_t scale = nactive ? 1.0/nactive : 0.0;
		while (n--) *dst++ *= scale;

		delete [] voice_buffer;
	}

	void on_note_off(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) 
	{
		for (std::vector<Voice*>::iterator i = _voices.begin(); i != _voices.end(); ++i)
			if ((*i)->note_playing() == note)
				(*i)->stop_note();
	}

	void on_note_on(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) 
	{
		for (std::vector<Voice*>::iterator i = _voices.begin(); i != _voices.end(); ++i)
		{
			if ((*i)->note_playing() == 0)
			{
				(*i)->play_note(note);
				break;
			}
		}
	}

private:
	std::vector<Voice*> _voices;
	jack_default_audio_sample_t** _voice_buffers;
};


int 
main(int argc, char **argv)
{
	Demo4 client;

	while (1)
		sleep(1);
}
