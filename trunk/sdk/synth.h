#ifndef NUCLEAR_SYNTH_H
#define NUCLEAR_SYNTH_H

// Nuclear
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

#include "synth_descriptor.h"

namespace nuclear
{

class synth
{
public:
        synth() {}
        virtual ~synth() {}

	/// Note off handler
	virtual void note_off(note_t note, velocity_t velocity) = 0;
	/// Note on handler
	virtual void note_on(note_t note, velocity_t velocity) = 0;
	/// Aftertouch handler
	virtual void aftertouch(note_t note, velocity_t velocity) = 0;
	/// Channel pressure handler
	virtual void channel_pressure(velocity_t pressure) = 0;
	/// Pitch bend handler
	virtual void pitch_bend(velocity_t value) = 0;
	/// Program change handler
	virtual void program_change(uint32_t bank, uint32_t program) = 0;
	/// Configuration handler
	virtual void configure(string_t key, string_t value) = 0;

	//todo: add named callbacks for continous controllers

	/// Connect data to port
	virtual void connect_port(port_t index, float_t* data) = 0;

	/// Initate synth
	virtual void init(uint32_t srate) = 0;
	/// Run synth
	virtual void run(nframes_t nframes) = 0;

	/// Build descriptor
	static void build_descriptor(nuclear::synth_descriptor* descriptor);
};

} // !namespace nuclear

#endif // !NUCLEAR_SYNTH_H
