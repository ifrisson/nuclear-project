#ifndef NUCLEAR_DSSI_PLUGIN_H
#define NUCLEAR_DSSI_PLUGIN_H

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

#include <vector>
#include <stdlib.h>
#include <string.h>
#include <ladspa.h>
#include <dssi.h>
#include "../synth.h"
#include "../synth_descriptor.h"
#include "../types.h"

////////////////////////////////////////////////////////////////
// Synth

class synth_factory
{
public:
	virtual nuclear::synth* factory() = 0;
};

static synth_factory* g_synth_factory = NULL;

////////////////////////////////////////////////////////////////
// Plugin

static LADSPA_Handle instantiate(const LADSPA_Descriptor* descriptor, unsigned long srate) 
{
	nuclear::synth* plugin = g_synth_factory->factory();
	plugin->init(srate);
	return plugin;
}

static void cleanup(LADSPA_Handle instance) 
{
	nuclear::synth* plugin = static_cast<nuclear::synth*>(instance);
	delete plugin;
}

static void activate(LADSPA_Handle instance)
{
	//fixme: implement!
}

static void deactivate(LADSPA_Handle instance)
{
	//fixme: implement!
}

static void connect_port(LADSPA_Handle instance, unsigned long port, LADSPA_Data* data) 
{
	nuclear::synth* plugin = static_cast<nuclear::synth*>(instance);
	plugin->connect_port(port, data);
}

static int get_midi_controller_for_port(LADSPA_Handle instance, unsigned long port)
{
	//fixme: implement!
	return -1;
}

static void run_synth(LADSPA_Handle instance, unsigned long nframes, snd_seq_event_t* events, unsigned long event_count)
{
	nuclear::synth* plugin = static_cast<nuclear::synth*>(instance);

	// Process MIDI
	for (int event_index = 0; event_index < event_count; ++event_index)
	{
		if (events[event_index].type == SND_SEQ_EVENT_NOTEOFF)
		{
			plugin->note_off( 
				events[event_index].data.note.note,
				events[event_index].data.note.velocity
				);
		}
		else if (events[event_index].type == SND_SEQ_EVENT_NOTEON)
		{
			plugin->note_on(
				events[event_index].data.note.note,
				events[event_index].data.note.velocity
				);
		}
		else if (events[event_index].type == SND_SEQ_EVENT_KEYPRESS)
		{
			plugin->aftertouch(
				events[event_index].data.note.note,
				events[event_index].data.note.velocity
				);
		}
/*
		//note: the host will not deliver bank changes nor controllers already mapped
		else if (events[event_index].type == SND_SEQ_EVENT_CONTROLLER)
		{
			//todo: add named callbacks for continous controllers
			plugin->continous_controller(
				events[event_index].data.control.param,
				events[event_index].data.control.value
				);
		}
*/
		else if (events[event_index].type == SND_SEQ_EVENT_CHANPRESS)
		{
			plugin->channel_pressure(
				events[event_index].data.control.value
				);
		}
		else if (events[event_index].type == SND_SEQ_EVENT_PITCHBEND)
		{
			plugin->pitch_bend(
				events[event_index].data.control.value
				);
		}
	}

	// Process audio
	plugin->run(nframes);
}

static void run(LADSPA_Handle instance, unsigned long nframes) 
{
	run_synth(instance, nframes, NULL, 0);
}

static char* configure(LADSPA_Handle instance, const char* key, const char* value)
{
	nuclear::synth* plugin = static_cast<nuclear::synth*>(instance);
	plugin->configure(nuclear::string_t(key), nuclear::string_t(value));
	return NULL;
}

static const DSSI_Program_Descriptor* get_program(LADSPA_Handle instance, unsigned long index)
{
	nuclear::synth* plugin = static_cast<nuclear::synth*>(instance);
	//fixme: implement!
	//return plugin->get_program(index);
	return NULL;
}

static void select_program(LADSPA_Handle instance, unsigned long bank, unsigned long program)
{
	nuclear::synth* plugin = static_cast<nuclear::synth*>(instance);
	plugin->program_change(bank, program);

	// A plugin is permitted to re-write the values of its input
	// control ports when select_program is called.
}

////////////////////////////////////////////////////////////////
// Descriptors

static LADSPA_Descriptor* g_ladspa_descriptor = NULL;
const LADSPA_Descriptor* ladspa_descriptor(unsigned long index) 
{
	return g_ladspa_descriptor;
}

static DSSI_Descriptor* g_dssi_descriptor = NULL;
const DSSI_Descriptor* dssi_descriptor(unsigned long index)
{
	return g_dssi_descriptor;
}

namespace
{

class descriptor_builder :
	public nuclear::synth_descriptor
{
	struct port
	{
		port(nuclear::string_t name, int descriptor, int hints, float low, float high) :
			name(name),
			descriptor(descriptor),
			hints(hints),
			low(low),
			high(high)
		{}

		nuclear::string_t name;
		int descriptor;
		int hints;
		float low;
		float high;
	};

public:
	void create_new_descriptor()
	{
		_descriptor = new LADSPA_Descriptor;
	}

	LADSPA_Descriptor* get_descriptor()
	{
		_descriptor->PortCount = _ports.size();

		char** port_names = (char **)calloc(_ports.size(), sizeof(char *));
		_descriptor->PortNames = (const char **)port_names;

		LADSPA_PortDescriptor* port_descriptors = (LADSPA_PortDescriptor *)calloc(_ports.size(), sizeof(LADSPA_PortDescriptor));
		_descriptor->PortDescriptors = (const LADSPA_PortDescriptor *)port_descriptors;

		LADSPA_PortRangeHint* port_range_hints = ((LADSPA_PortRangeHint *)calloc(_ports.size(), sizeof(LADSPA_PortRangeHint)));
		_descriptor->PortRangeHints = (const LADSPA_PortRangeHint *)port_range_hints;

		int p = 0;
		for (std::vector<port>::iterator i = _ports.begin(); i != _ports.end(); ++i)
		{
			port_names[p] = strdup((*i).name.c_str());
			port_descriptors[p] = (*i).descriptor;
			port_range_hints[p].HintDescriptor = (*i).hints;
			port_range_hints[p].LowerBound = (*i).low;
			port_range_hints[p].UpperBound = (*i).high;
			p++;
		}

		return _descriptor;
	}

	void set_label(nuclear::string_t label)
	{
		_descriptor->Label = label.c_str();
	}

	void set_name(nuclear::string_t name)
	{
		_descriptor->Name = name.c_str();
	}

	void set_maker(nuclear::string_t maker)
	{
		_descriptor->Maker = maker.c_str();
	}

	void set_copyright(nuclear::string_t copyright)
	{
		_descriptor->Copyright = copyright.c_str();
	}

	void add_control(nuclear::string_t name, float min, float max)
	{
		_ports.push_back(port(name, (LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL), 0, min, max));
	}

	void add_input(nuclear::string_t name)
	{
		_ports.push_back(port(name, (LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO), 0, 0, 0));
	}

	void add_output(nuclear::string_t name)
	{
		_ports.push_back(port(name, (LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO), 0, 0, 0));
	}

private:
	LADSPA_Descriptor* _descriptor;
	std::vector<port> _ports;
};

template<typename T>
class concrete_synth_factory :
	public synth_factory
{
public:
	nuclear::synth* factory()
	{
		return new T;
	}
};

} // !namespace

namespace nuclear
{

template <typename T>
inline void register_dssi()
{
	descriptor_builder* builder = new descriptor_builder();
	builder->create_new_descriptor();
	T::build_descriptor(builder);

	g_ladspa_descriptor = builder->get_descriptor();
	g_ladspa_descriptor->UniqueID = 0;
	g_ladspa_descriptor->instantiate = instantiate;
	g_ladspa_descriptor->connect_port = connect_port;
	g_ladspa_descriptor->activate = activate;
	g_ladspa_descriptor->run = run;
	g_ladspa_descriptor->run_adding = NULL;
	g_ladspa_descriptor->set_run_adding_gain = NULL;
	g_ladspa_descriptor->deactivate = deactivate;
	g_ladspa_descriptor->cleanup = cleanup;

	g_dssi_descriptor = new DSSI_Descriptor;
	g_dssi_descriptor->DSSI_API_Version = 1;
	g_dssi_descriptor->LADSPA_Plugin = g_ladspa_descriptor;
	g_dssi_descriptor->configure = configure;
	g_dssi_descriptor->get_program = get_program;
	g_dssi_descriptor->select_program = select_program;
	g_dssi_descriptor->get_midi_controller_for_port = get_midi_controller_for_port;
	g_dssi_descriptor->run_synth = run_synth;
	g_dssi_descriptor->run_synth_adding = NULL;
	g_dssi_descriptor->run_multiple_synths = NULL;
	g_dssi_descriptor->run_multiple_synths_adding = NULL;

	g_synth_factory = new concrete_synth_factory<T>();
}

} // !namespace nuclear

#define NUCLEAR_REGISTER_DSSI(T) void _init() { nuclear::register_dssi<T>(); }

#endif // !NUCLEAR_DSSI_PLUGIN_H
