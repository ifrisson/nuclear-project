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

#include "types.h"

namespace nuclear
{

class voice
{
public:
        voice() {}
        virtual ~voice() {}
	
	/// Start playing a note
        virtual void play_note(note_t note, velocity_t velocity) = 0;
	/// Stop playing the current note
	virtual void stop_note() = 0;
	/// Kill the note and free the voice immediately
	virtual void kill_note() = 0;
	/// Return the note playing or 0 if voice is inactive
	virtual note_t note_playing() = 0;
	/// Return the note time stamp or 0 if voice is inactive
	virtual double_t note_timestamp() = 0;

	/// Return number of audio input buffers process_audio expects
        static port_t expected_audio_inputs();
	/// Return number of audio output buffers process_audio expects
        static port_t expected_audio_outputs();
	/// Initiate processor, must be called at least once before processing
        virtual void init(uint32_t srate) = 0;
	/// Process audio
        virtual void process_audio(nframes_t nframes, sample_t** inputs, sample_t** outputs) = 0;
};

} // !namespace nuclear

#endif // !NUCLEAR_VOICE_H
