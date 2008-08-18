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

#include <ladspa.h>
#include <dssi.h>
#include "../../engine.h"
#include "../../types.h"
#include "dssi_descriptor_builder.h"

static LADSPA_Descriptor* g_ladspa_descriptor = NULL;
const LADSPA_Descriptor* ladspa_descriptor(unsigned long index) { return g_ladspa_descriptor; }

static DSSI_Descriptor* g_dssi_descriptor = NULL;
const DSSI_Descriptor* dssi_descriptor(unsigned long index) { return g_dssi_descriptor; }

namespace nuclear { 

	namespace dssi {
		
		/// Wrap engine as DSSI plugin
		template<typename T>
		class plugin
		{
		public:
			plugin(descriptor_builder* builder)
			{
				g_ladspa_descriptor = builder->get_descriptor();
				g_ladspa_descriptor->UniqueID = 0;
				g_ladspa_descriptor->instantiate = dssi_plugin::instantiate;
				g_ladspa_descriptor->connect_port = dssi_plugin::connect_port;
				g_ladspa_descriptor->activate = dssi_plugin::activate;
				g_ladspa_descriptor->run = dssi_plugin::run;
				g_ladspa_descriptor->run_adding = NULL;
				g_ladspa_descriptor->set_run_adding_gain = NULL;
				g_ladspa_descriptor->deactivate = dssi_plugin::deactivate;
				g_ladspa_descriptor->cleanup = dssi_plugin::cleanup;
				
				g_dssi_descriptor = new DSSI_Descriptor;
				g_dssi_descriptor->DSSI_API_Version = 1;
				g_dssi_descriptor->LADSPA_Plugin = g_ladspa_descriptor;
				g_dssi_descriptor->configure = dssi_plugin::configure;
				g_dssi_descriptor->get_program = dssi_plugin::get_program;
				g_dssi_descriptor->select_program = dssi_plugin::select_program;
				g_dssi_descriptor->get_midi_controller_for_port = dssi_plugin::get_midi_controller_for_port;
				g_dssi_descriptor->run_synth = dssi_plugin::run_synth;
				g_dssi_descriptor->run_synth_adding = NULL;
				g_dssi_descriptor->run_multiple_synths = NULL;
				g_dssi_descriptor->run_multiple_synths_adding = NULL;
			}
			
			static LADSPA_Handle instantiate(const LADSPA_Descriptor* descriptor, unsigned long srate) 
			{
				nuclear::engine* plugin = new T;
				plugin->init(srate);
				return plugin;
			}
			
			static void cleanup(LADSPA_Handle instance) 
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				delete plugin;
			}
			
			static void activate(LADSPA_Handle instance)
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				plugin->activate();
			}
			
			static void deactivate(LADSPA_Handle instance)
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				plugin->deactivate();
			}
			
			static void connect_port(LADSPA_Handle instance, unsigned long port, LADSPA_Data* data_location) 
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				plugin->connect_port(port, data_location);
			}
			
			static int get_midi_controller_for_port(LADSPA_Handle instance, unsigned long port)
			{
				//fixme: implement!
				return -1;
			}
			
			static void run_synth(LADSPA_Handle instance, unsigned long nframes, snd_seq_event_t* events, unsigned long event_count)
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				
				nuclear::midi* midi = dynamic_cast<nuclear::midi*>(plugin);
				if (midi)
				{
					for (int event_index = 0; event_index < event_count; ++event_index)
					{
						if (events[event_index].type == SND_SEQ_EVENT_NOTEOFF)
						{
							midi->note_off( 
								events[event_index].data.note.note,
								events[event_index].data.note.velocity
								);
						}
						else if (events[event_index].type == SND_SEQ_EVENT_NOTEON)
						{
							midi->note_on(
								events[event_index].data.note.note,
								events[event_index].data.note.velocity
								);
						}
						else if (events[event_index].type == SND_SEQ_EVENT_KEYPRESS)
						{
							midi->aftertouch(
								events[event_index].data.note.note,
								events[event_index].data.note.velocity
								);
						}
						/*
						//note: the host will not deliver bank changes nor controllers already mapped
						else if (events[event_index].type == SND_SEQ_EVENT_CONTROLLER)
						{
						//todo: add named callbacks for continous controllers
						midi->continous_controller(
						events[event_index].data.control.param,
						events[event_index].data.control.value
						);
						}
						*/
						else if (events[event_index].type == SND_SEQ_EVENT_CHANPRESS)
						{
							midi->channel_pressure(
								events[event_index].data.control.value
								);
						}
						else if (events[event_index].type == SND_SEQ_EVENT_PITCHBEND)
						{
							midi->pitch_bend(
								events[event_index].data.control.value
								);
						}
					}
				}
				
				plugin->run(nframes);
			}
			
			static void run(LADSPA_Handle instance, unsigned long nframes) 
			{
				dssi_plugin::run_synth(instance, nframes, NULL, 0);
			}
			
			static char* configure(LADSPA_Handle instance, const char* key, const char* value)
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				//fixme: implement!
				return NULL;
			}
			
			static const DSSI_Program_Descriptor* get_program(LADSPA_Handle instance, unsigned long index)
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				//fixme: implement!
				return NULL;
			}
			
			static void select_program(LADSPA_Handle instance, unsigned long bank, unsigned long program)
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				//fixnme: implement!
			}
			
		};
		
	} // !namespace dssi
	
} // !namespace nuclear

/// Macro to register engine as DSSI plugin
#define NUCLEAR_DSSI_PLUGIN(T, BUILDER) static void* _ = new nuclear::dssi::plugin<T>(BUILDER);

#endif // !NUCLEAR_DSSI_PLUGIN_H
