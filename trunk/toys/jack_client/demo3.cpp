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

class Voice
{
public:
	Voice() :
		_note(0),
		_frequency(0.0),
		_amplitude(0.0)
	{
	}

	void process(jack_nframes_t srate, jack_nframes_t nframes, jack_default_audio_sample_t* out)
	{
		jack_default_audio_sample_t F = (2 * M_PI * _frequency) / srate;
		for (int i = 0; i < nframes; ++i)
			out[i] = _amplitude * sin(F * i);
	}

	void note_on(jack_midi_data_t note)
	{
		_note = note;
		_frequency = note_to_frequency(_note);
		_amplitude = 1.0;
	}

	void note_off()
	{
		_note = 0;
		_frequency = 0.0;
		_amplitude = 0.0;
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
	jack_default_audio_sample_t _frequency;
	jack_default_audio_sample_t _amplitude;
};

class Demo3 :
	public JackClient
{
public:
	Demo3() :
		JackClient("demo3")
	{
		for (int i = 0; i < 8; ++i)
			_voices.push_back(new Voice());

		open_audio_out_ports(1);
		open_midi_in_ports(1);
		activate();
	}

	~Demo3()
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

		jack_default_audio_sample_t* voice_buffer = new jack_default_audio_sample_t[buffer_size()];
		for (int i = 0; i < _voices.size(); ++i)
		{
			_voices[i]->process(sample_rate(), buffer_size(), voice_buffer);
			jack_nframes_t n = buffer_size();
			jack_default_audio_sample_t* src = voice_buffer;
			jack_default_audio_sample_t* dst = out;
			while (n--) *dst++ += *src++;			
		}

		jack_nframes_t n = buffer_size();
		jack_default_audio_sample_t* dst = out;
		while (n--) *dst++ *= 1.0 / _voices.size();

		delete [] voice_buffer;
	}

	void on_note_off(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) 
	{
		for (std::vector<Voice*>::iterator i = _voices.begin(); i != _voices.end(); ++i)
			if ((*i)->note_playing() == note)
				(*i)->note_off();
	}

	void on_note_on(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) 
	{
		for (std::vector<Voice*>::iterator i = _voices.begin(); i != _voices.end(); ++i)
		{
			if ((*i)->note_playing() == 0)
			{
				(*i)->note_on(note);
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
	Demo3* client = new Demo3();

	while (1)
		sleep(1);
}
