#ifndef GRAPH_BLOCK_H
#define GRAPH_BLOCK_H

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

#include "types.h"

namespace nuclear
{

/////////////////////////////////////////////////////////////////////////////
// block

class block
{
public:
	/// Initiate block
	virtual void init(uint_32 srate) = 0;
	/// Return the number of inputs
	virtual port_t number_of_inputs() = 0;
	/// Return the number of outputs
	virtual port_t number_of_outputs() = 0;
	/// Produce a new buffer of samples
	virtual void process(const nframes_t nframes, sample_t** inputs, sample_t** outputs) = 0;
};

} // !namespace nuclear

#endif // !GRAPH_IBLOCK_H
