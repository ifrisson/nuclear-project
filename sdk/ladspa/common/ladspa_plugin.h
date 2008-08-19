/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef NUCLEAR_LADSPA_PLUGIN_H
#define NUCLEAR_LADSPA_PLUGIN_H

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

#include "../ladspa.h"
#include "../../engine.h"
#include "../../types.h"
#include "ladspa_descriptor_builder.h"

static LADSPA_Descriptor* g_ladspa_descriptor = NULL;
const LADSPA_Descriptor* ladspa_descriptor(unsigned long index) 
{
	if (index > 0)
		return NULL;
	return g_ladspa_descriptor; 
}

namespace nuclear { 

	namespace ladspa {
		
		/// Wrap engine as LADSPA plugin
		template<typename T>
		class plugin
		{
		public:
			plugin(descriptor_builder* builder)
			{
				g_ladspa_descriptor = builder->get_descriptor();
				g_ladspa_descriptor->UniqueID = 0;
				g_ladspa_descriptor->instantiate = ladspa_plugin::instantiate;
				g_ladspa_descriptor->connect_port = ladspa_plugin::connect_port;
				g_ladspa_descriptor->activate = ladspa_plugin::activate;
				g_ladspa_descriptor->run = ladspa_plugin::run;
				g_ladspa_descriptor->run_adding = NULL;
				g_ladspa_descriptor->set_run_adding_gain = NULL;
				g_ladspa_descriptor->deactivate = ladspa_plugin::deactivate;
				g_ladspa_descriptor->cleanup = ladspa_plugin::cleanup;
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
			
			static void run(LADSPA_Handle instance, unsigned long nframes) 
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				plugin->run(nframes);
			}
			
		};
		
	} // !namespace ladspa
	
} // !namespace nuclear

/// Macro to register engine as LADSPA plugin
#define NUCLEAR_LADSPA_PLUGIN(T, BUILDER) static void* _ = new nuclear::ladspa::plugin<T>(BUILDER);

#endif // !NUCLEAR_LADSPA_PLUGIN_H
