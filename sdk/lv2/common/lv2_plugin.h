#ifndef NUCLEAR_LV2_PLUGIN_H
#define NUCLEAR_LV2_PLUGIN_H

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

#include "../lv2.h"
#include "../../types.h"
#include "../../engine.h"

static LV2_Descriptor* g_lv2_descriptor = NULL;
const LV2_Descriptor* lv2_descriptor(uint32_t index) { return g_lv2_descriptor; }

namespace nuclear
{

	namespace lv2
	{

		/// Wrap engine as LV2 plugin
		template<typename T>
		class plugin
		{
		public:
			plugin(const string_t URI)
			{
				g_lv2_descriptor = new LV2_Descriptor;
				g_lv2_descriptor->URI = URI.c_str();
				g_lv2_descriptor->instantiate = lv2_plugin::instantiate;
				g_lv2_descriptor->connect_port = lv2_plugin::connect_port;
				g_lv2_descriptor->activate = lv2_plugin::activate;
				g_lv2_descriptor->run = lv2_plugin::run;
				g_lv2_descriptor->deactivate = lv2_plugin::deactivate;
				g_lv2_descriptor->cleanup = lv2_plugin::cleanup;
				g_lv2_descriptor->extension_data = lv2_plugin::extension_data;
			}
			
			static LV2_Handle instantiate(const struct _LV2_Descriptor* descriptor,
						      double                        sample_rate,
						      const char*                   bundle_path,
						      const LV2_Feature* const*     features)
			{
				nuclear::engine* plugin = new T;
				plugin->init(sample_rate);
				return plugin;
			}
		
			static void connect_port(LV2_Handle instance, uint32_t port, void* data_location)
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				plugin->connect_port(port, data_location);
			}
		
			static void activate(LV2_Handle instance)
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				plugin->activate();
			}
		
			static void run(LV2_Handle instance, uint32_t sample_count)
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				plugin->run(sample_count);
			}
		
			static void deactivate(LV2_Handle instance)
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				plugin->deactivate();
			}
		
			static void cleanup(LV2_Handle instance)
			{
				nuclear::engine* plugin = static_cast<nuclear::engine*>(instance);
				delete plugin;
			}
		
			static const void* extension_data(const char* uri)
			{
			}
		};

	} // !namespace lv2
		
} // !namespace nuclear

/// Macro to register engine as LV2 plugin		
#define NUCLEAR_LV2_PLUGIN(T, URI) static void* _ = new nuclear::lv2::plugin<T>(URI);
		
#endif // !NUCLEAR_LV2_PLUGIN_H
		
