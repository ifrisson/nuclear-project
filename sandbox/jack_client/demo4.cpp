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
#include <math.h>
#include <map>

class UI
{
public:
		
	UI() {}
	virtual ~UI() {}
	
	// -- active widgets
	
	virtual void addButton(std::string label, float* zone) = 0;
	virtual void addToggleButton(std::string label, float* zone) = 0;
	virtual void addCheckButton(std::string label, float* zone) = 0;
	virtual void addVerticalSlider(std::string label, float* zone, float init, float min, float max, float step) = 0;
	virtual void addHorizontalSlider(std::string label, float* zone, float init, float min, float max, float step) = 0;
	virtual void addNumEntry(std::string label, float* zone, float init, float min, float max, float step) = 0;
	
	// -- passive widgets
	
	virtual void addNumDisplay(std::string label, float* zone, int precision) = 0;
	virtual void addTextDisplay(std::string label, float* zone, char* names[], float min, float max) = 0;
	virtual void addHorizontalBargraph(std::string label, float* zone, float min, float max) = 0;
	virtual void addVerticalBargraph(std::string label, float* zone, float min, float max) = 0;
	
	// -- frames and labels
	
	virtual void openFrameBox(std::string label) = 0;
	virtual void openTabBox(std::string label) = 0;
	virtual void openHorizontalBox(std::string label) = 0;
	virtual void openVerticalBox(std::string label) = 0;
	virtual void closeBox() = 0;
};

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

class dsp 
{
protected:
        int fSamplingFreq;
public:
        dsp() {}
        virtual ~dsp() {}
	
        virtual int getNumInputs() = 0;
        virtual int getNumOutputs() = 0;
        virtual void buildUserInterface(UI* interface)  = 0;
        virtual void init(int samplingRate) = 0;
        virtual void compute(int len, float** inputs, float** outputs)  = 0;
};

// Class generated from osci.dsp
class mydsp : 
	public dsp 
{
private:
	class SIG0 
	{
	private:
		int 	fSamplingFreq;
		int 	iRec1[2];
	public:
		int getNumInputs() 	{ return 0; }
		int getNumOutputs() 	{ return 1; }

		void init(int samplingFreq) 
		{
			fSamplingFreq = samplingFreq;
			for (int i=0; i<2; i++) iRec1[i] = 0;
		}

		void fill (int count, float output[]) 
		{
			for (int i=0; i<count; i++) {
				iRec1[0] = (1 + iRec1[1]);
				output[i] = sinf((9.587380e-05f * float((iRec1[0] - 1))));
				// post processing
				iRec1[1] = iRec1[0];
			}
		}
	};
	
	float 	fslider0;
	float 	fConst0;
	float 	fRec0[2];
	static float 	ftbl0[65537];
	float 	fslider1;
	float 	fRec2[2];
public:
	virtual int getNumInputs() 	{ return 0; }
	virtual int getNumOutputs() 	{ return 1; }

	static void classInit(int samplingFreq) 
	{
		SIG0 sig0;
		sig0.init(samplingFreq);
		sig0.fill(65537,ftbl0);
	}

	virtual void instanceInit(int samplingFreq) 
	{
		fSamplingFreq = samplingFreq;
		fslider0 = 1000.000000f;
		fConst0 = (1.000000f / float(fSamplingFreq));
		for (int i=0; i<2; i++) fRec0[i] = 0;
		fslider1 = 0.000000f;
		for (int i=0; i<2; i++) fRec2[i] = 0;
	}

	virtual void init(int samplingFreq) 
	{
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}

	virtual void buildUserInterface(UI* interface) 
	{
		interface->openVerticalBox("Oscillator");
		interface->addHorizontalSlider("freq", &fslider0, 1000.000000f, 0.000000f, 24000.000000f, 0.100000f);
		interface->addHorizontalSlider("volume", &fslider1, 0.000000f, -96.000000f, 0.000000f, 0.100000f);
		interface->closeBox();
	}

	virtual void compute (int count, float** input, float** output) 
	{
		float* output0 = output[0];
		float fSlow0 = (fConst0 * fslider0);
		float fSlow1 = (9.999871e-04f * powf(10, (5.000000e-02f * fslider1)));
		for (int i=0; i<count; i++) {
			float fTemp0 = (fSlow0 + fRec0[1]);
			fRec0[0] = (fTemp0 - floorf(fTemp0));
			float fTemp1 = (65536.000000f * fRec0[0]);
			int iTemp2 = int(fTemp1);
			float fTemp3 = ftbl0[iTemp2];
			fRec2[0] = (fSlow1 + (0.999000f * fRec2[1]));
			output0[i] = (fRec2[0] * (fTemp3 + ((ftbl0[(1 + iTemp2)] - fTemp3) * (fTemp1 - floorf(fTemp1)))));
			// post processing
			fRec2[1] = fRec2[0];
			fRec0[1] = fRec0[0];
		}
	}
};

float 	mydsp::ftbl0[65537];

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

		jack_default_audio_sample_t* voice_buffer = new jack_default_audio_sample_t[buffer_size()];
		for (int i = 0; i < _voices.size(); ++i)
		{
			_voices[i]->process(buffer_size(), voice_buffer);
			jack_nframes_t n = buffer_size();
			jack_default_audio_sample_t* src = voice_buffer;
			jack_default_audio_sample_t* dst = out;
			while (n--) *dst++ += *src++;			
		}

		jack_nframes_t n = buffer_size();
		jack_default_audio_sample_t* dst = out;
		jack_default_audio_sample_t scale = 1.0 / _voices.size();
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
