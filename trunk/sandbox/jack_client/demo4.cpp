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
#include "../../sdk/UI.h"
#include <math.h>
#include <map>

using nuclear::dsp;
using nuclear::UI;

// Include the generated file
#include "osci.dsp.in"

struct param {
	float* fZone; float fMin; float fMax;
	param(float* z, float a, float b) : fZone(z), fMin(a), fMax(b) {}
};

class DEMOUI :
	public UI
{
public:
	DEMOUI() : 
		UI()
	{
	}

	virtual ~DEMOUI() {}

	virtual void addButton(std::string label, float* zone) {}
	virtual void addToggleButton(std::string label, float* zone) {}
	virtual void addCheckButton(std::string label, float* zone) {}
	virtual void addVerticalSlider(std::string label, float* zone, float init, float min, float max, float step) {}

	virtual void addHorizontalSlider(std::string label, float* zone, float init, float min, float max, float step) 
	{
		add_option(label, zone, min, max);
	}

	virtual void addNumEntry(std::string label, float* zone, float init, float min, float max, float step) {}
	
	virtual void addNumDisplay(std::string label, float* zone, int precision) {}
	virtual void addTextDisplay(std::string label, float* zone, char* names[], float min, float max) {}
	virtual void addHorizontalBargraph(std::string label, float* zone, float min, float max) {}
	virtual void addVerticalBargraph(std::string label, float* zone, float min, float max) {}
	
	virtual void openFrameBox(std::string label) {}
	virtual void openTabBox(std::string label) {}
	virtual void openHorizontalBox(std::string label) {}
	virtual void openVerticalBox(std::string label) {}
	virtual void closeBox() {}

	void set_option(std::string label, float value)
	{
		std::map<std::string, param>::iterator p = _params.find(label);
		if (p == _params.end())
			throw "Unrecognized option";

		if (value >= p->second.fMin && value <= p->second.fMax)
			*(p->second.fZone) = value;
		else
			std::cerr << "Can't set option " << label << ": " << value << " (" << p->second.fMin << ", " << p->second.fMax << ")" << std::endl;
	}

private:
	void add_option(std::string label, float* zone, float min, float max)
	{
		_params.insert(std::make_pair(label, param(zone, min, max)));
	}

	std::map<std::string, param> _params;
};

class Voice
{
public:
	Voice(jack_nframes_t srate) :
		_note(0),
		_interface(new DEMOUI())
	{
		_dsp.init(srate);
		_dsp.buildUserInterface(_interface);
		_interface->set_option("freq", 0.000000f);
		_interface->set_option("volume", -96.000000f);
	}

	void process(jack_nframes_t nframes, jack_default_audio_sample_t* out)
	{
		jack_default_audio_sample_t* output[] = { out };
		_dsp.compute(nframes, NULL, output);
	}

	void note_on(jack_midi_data_t note)
	{
		_note = note;
		_interface->set_option("freq", note_to_frequency(_note));
		_interface->set_option("volume", 0.000000f);
	}

	void note_off()
	{
		_note = 0;
		_interface->set_option("freq", 0.000000f);
		_interface->set_option("volume", -96.000000f);
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
	DEMOUI* _interface;
};

class Demo4 :
	public nuclear::jack_client
{
public:
	Demo4() :
		nuclear::jack_client("demo4")
	{
		for (int i = 0; i < 8; ++i)
			_voices.push_back(new Voice(sample_rate()));

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

			(*i)->process(buffer_size(), voice_buffer);
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
	Demo4 client;

	while (1)
		sleep(1);
}
