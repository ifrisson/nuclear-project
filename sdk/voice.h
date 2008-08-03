#ifndef NUCLEAR_VOICE_H
#define NUCLEAR_VOICE_H

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

#include "dsp.h"
#include "types.h"

namespace nuclear
{

class voice :
	public dsp
{
public:
        voice() {}
        virtual ~voice() {}
	
	/// Start playing a note
        virtual void play_note(uint8_t note) = 0;
	/// Stop playing the current note
	virtual void stop_note() = 0;
	/// Kill the note and free the voice immediately
	virtual void kill_note() = 0;
	/// Return the note playing or 0 if voice is inactive
	virtual uint8_t note_playing() = 0;
};

} // !namespace nuclear

#endif // !NUCLEAR_VOICE_H
