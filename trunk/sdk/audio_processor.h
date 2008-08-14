#ifndef NUCLEAR_AUDIO_PROCESSOR_H
#define NUCLEAR_AUDIO_PROCESSOR_H

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

/// Process audio using zero or more inputs/outputs
class audio_processor
{
public:
        audio_processor() {}
        virtual ~audio_processor() {}
	
	/// Return number of audio input ports
        virtual port_t get_num_audio_inputs() = 0;
	/// Return number of audio output ports
        virtual port_t get_num_audio_outputs() = 0;
	/// Initiate processor, must be called at least once before processing
        virtual void init(uint32_t srate) = 0;
	/// Process audio
        virtual void process_audio(nframes_t nframes, sample_t** inputs, sample_t** outputs) = 0;
};

} // !namespace nuclear

#endif // !NUCLEAR_AUDIO_PROCESSOR_H
