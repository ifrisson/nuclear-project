#ifndef GRAPH_FAUST_H
#define GRAPH_FAUST_H

// Graph Demo
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

#include  "block.h"

///////////////////////////////////////////////////////////////
// UI

class UI
{
private:
	bool fStopped;

public:
		
	UI() : fStopped(false) {}
	virtual ~UI() {}
	
	virtual void addButton(char* label, float* zone) = 0;
	virtual void addToggleButton(char* label, float* zone) = 0;
	virtual void addCheckButton(char* label, float* zone) = 0;
	virtual void addVerticalSlider(char* label, float* zone, float init, float min, float max, float step) = 0;
	virtual void addHorizontalSlider(char* label, float* zone, float init, float min, float max, float step) = 0;
	virtual void addNumEntry(char* label, float* zone, float init, float min, float max, float step) = 0;
	
	virtual void openFrameBox(char* label) = 0;
	virtual void openTabBox(char* label) = 0;
	virtual void openHorizontalBox(char* label) = 0;
	virtual void openVerticalBox(char* label) = 0;
	virtual void closeBox() = 0;
	
	virtual void run() = 0;
	
	void stop()	{ fStopped = true; }
	bool stopped() 	{ return fStopped; }
};


///////////////////////////////////////////////////////////////
// dsp

class dsp :
	public block
{
protected:
	int fSamplingFreq;

public:
	dsp() {}
	virtual ~dsp() {}
	
	virtual int getNumInputs() = 0;
	virtual int getNumOutputs() = 0;
	virtual void buildUserInterface(UI* interface) = 0;
	virtual void init(int samplingRate) = 0;
 	virtual void compute(int len, float** inputs, float** outputs) = 0;

	void set_samplerate(uint_32 srate) { init(srate); }
	port_t number_of_inputs() { return getNumInputs(); }
	port_t number_of_outputs() { return getNumOutputs(); }
	void process(const nframes_t nframes, sample_t** inputs, sample_t** outputs) 
	{ 
		compute(nframes, inputs, outputs); 
	}
};


#endif // !GRAPH_FAUST_H
