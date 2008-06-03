// Vivace Performer
// Copyright (c) 2008, Anders Dahnielson
//
// Contact: anders@dahnielson.com

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
