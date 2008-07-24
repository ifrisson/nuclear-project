#ifndef PERFORMER_SEQUENCER_H
#define PERFORMER_SEQUENCER_H

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

#include "commands.h"
#include "jack_client.h"
#include <map>

class Sequencer :
	public JackClient
{
public:
	typedef std::multimap<const jack_nframes_t, Command*> event_map;
	typedef std::pair<const jack_nframes_t, Command*> event_pair;

	Sequencer();
	void process(jack_nframes_t nframes);
	void set_command(const jack_nframes_t time, Command* command);
	void panic();
	void clear();

private:
	event_map events;
	bool was_rolling;
};

#endif // !PERFORMER_SEQUENCER_H
