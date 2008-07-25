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

#include "engine.h"
#include "utility.h"

namespace nuclear
{

/////////////////////////////////////////////////////////////////////////////
// engine

engine::engine(const string_t name, voice_builder* builder, const voice_t nvoices, const uint_32 srate) :
	name(name),
	nvoices(nvoices),
	ninputs(builder->number_of_inputs()),
	noutputs(builder->number_of_outputs())
{
	voice_director* director = new voice_director();
	director->set_voice_builder(builder);

	for (voice_t v = 0; v < nvoices; ++v)
	{
		director->construct_voice(srate);
		voices.push_back(director->get_voice());
	}
}

void 
engine::process(const nframes_t nframes, sample_t** inputs, sample_t** outputs)
{
	// allocate the voice buffer memory
	sample_t** voice_buffers = allocate_buffers<sample_t>(nvoices * noutputs, nframes);
	
	// process all voices
	sample_t* out_channels = new sample_t[noutputs];
	for (voice_t v = 0; v < nvoices; ++v)
	{
		for (port_t p = 0; p < noutput; ++p)
			out_channels[p] = voice_buffers[v*p];
		voices[v]->process(nframes, inputs, out_channels);
	}
	delete [] out_channels;
	
	// mix down all voices for each output
	sample_t* in_channels = new sample_t[noutputs];
	for (port_t p = 0; p < noutput; ++p)
	{
		for (voice_t v = 0; v < nvoices; ++v)
			in_channels[v] = voice_buffers[v*p];
		mixdown<sample_t>(nframes, in_channels, outputs[p]);
	}
	delete [] in_channels;
	
	// free the voice buffer memory
	free_buffers<sample_t>(voice_buffers);
}

} // !namespace nuclear
