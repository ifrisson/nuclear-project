#ifndef PERFORMER_SEQUENCER_H
#define PERFORMER_SEQUENCER_H

// Vivace Performer
// Copyright (c) 2008, Anders Dahnielson
//
// Contact: anders@dahnielson.com

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
