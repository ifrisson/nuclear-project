#ifndef NUCLEAR_MIDI_PROCESSOR_H
#define NUCLEAR_MIDI_PROCESSOR_H

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

#include "types.h"

namespace nuclear
{

class midi_processor
{
public:
        midi_processor() {}
        virtual ~midi_processor() {}
	
	virtual void note_off(int port, uint8_t channel, uint8_t note, uint8_t velocity) = 0;
	virtual void note_on(int port, uint8_t channel, uint8_t note, uint8_t velocity) = 0;
	virtual void aftertouch(int port, uint8_t channel, uint8_t note, uint8_t velocity) = 0;
	virtual void continous_controller(int port, uint8_t channel, uint8_t controller, uint8_t value) = 0;
	virtual void patch_change(int port, uint8_t channel, uint8_t program) = 0;
	virtual void channel_pressure(int port, uint8_t channel, uint8_t pressure) = 0;
	virtual void pitch_bend(int port, uint8_t channel, uint8_t value_lo, uint8_t value_hi) = 0;
};

} // !namespace nuclear

#endif // !NUCLEAR_MIDI_PROCESSOR_H
