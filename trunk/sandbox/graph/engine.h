#ifndef GRAPH_ENGINE_H
#define GRAPH_ENGINE_H

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

#include "voice.h"
#include "types.h"

namespace nuclear
{

/////////////////////////////////////////////////////////////////////////////
// engine

class engine
{
public:
	engine(const string_t name, voice_builder* builder, const voice_t nvoices, const uint_32 srate);
	~engine() {}
	const string_t name() { return name; }
	const voice_t max_number_of_voices() { return nvoices; }
	const port_t number_of_inputs() { return ninputs; }
	const port_t number_of_outputs() { return noutputs; }
	void process(const nframes_t nframes, sample_t** inputs, sample_t** outputs);

private:
	const string_t name;
	const voice_t nvoices;
	const port_t ninputs;
	const port_t noutputs;
	std::vector<voice*> voices;
};

} // !namespace nuclear

#endif // !GRAPH_ENGINE_H
