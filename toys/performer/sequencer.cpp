// Vivace Performer
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

#include "sequencer.h"
#include <iostream>

Sequencer::Sequencer() :
	JackClient("performer")
{
}

void 
Sequencer::process(jack_nframes_t nframes)
{
	if (!transport_rolling())
	{
		if (was_rolling)
		{
			panic();
			was_rolling = false;
		}
		return;
	}

	jack_nframes_t start_pos = transport_get_location();
	jack_nframes_t end_pos = start_pos + nframes;
	
	for (event_map::iterator i = events.lower_bound(start_pos); i != events.upper_bound(end_pos); ++i)
		(*i).second->execute();

	was_rolling = true;
}

void 
Sequencer::set_command(const jack_nframes_t time, Command* command)
{
	events.insert(event_pair(time, command));
}

void 
Sequencer::panic()
{
	for (int port = 0; port < ports; ++port)
		for (int ch = 0; ch < 16; ++ch)
			continous_controller(port, ch, 0x78, 0);
}

void
Sequencer::clear()
{
	events.clear();
}
