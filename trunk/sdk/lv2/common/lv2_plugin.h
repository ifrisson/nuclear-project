/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
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
#include "../lv2_event.h"
#include "../lv2_event_helpers.h"
#include "../lv2_uri_map.h"
#include "../../types.h"
#include "../../engine.h"

static LV2_Descriptor* g_lv2_descriptor = NULL;
const LV2_Descriptor* lv2_descriptor(uint32_t index) 
{ 
	if (index > 0)
		return NULL;
	return g_lv2_descriptor; 
}

namespace nuclear
{

	namespace lv2
	{

		/// Wrap engine as LV2 plugin
		template<typename T>
		class plugin
		{
			// Event extension
			LV2_Event_Buffer* event_buffer;
			LV2_Event_Feature* event_feature;

			// MIDI extension
			int midi_event_id;

			// URI-Map extension
			LV2_URI_Map_Feature* map_feature;

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
						      double sample_rate, 
						      const char* bundle_path, 
						      const LV2_Feature* const* features)
			{
				nuclear::engine* plugin = new T;

				// Look for features
				for(int i = 0; features[i]; ++i)
				{
					if (nuclear::string_t(features[i]->URI) == nuclear::string_t(LV2_URI_MAP_URI))
					{
						map_feature = features[i]->data;
						midi_event_id = map_feature->uri_to_id(map_feature->callback_data, LV2_EVENT_URI, "http://lv2plug.in/ns/ext/midi#MidiEvent");
					}
					else if (nuclear::string_t(features[i]->URI) == nuclear::string_t(LV2_EVENT_URI))
					{
						event_feature = features[i]->data;
					}
				}

				// Do we require MIDI?
				if (dynamic_cast<nuclear::midi*>(plugin))
				{
					if (midi_event_id == 0 || event_feature == NULL)
					{
						delete plugin;
						return NULL;
					}
				}

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

				// MIDI
				nuclear::midi* midi = dynamic_cast<nuclear::midi*>(plugin);
				if (midi)
				{
					const nuclear::float_t velocity_scale = 1.0f / 127;
					LV2_Event_Iterator iterator;
					for(lv2_event_begin(&iterator, event_buffer); 
					    lv2_event_is_valid(&iterator);
					    lv2_event_increment(&iterator))
					{
						uint16_t size = 0;
						if (lv2_event_get(&iterator, NULL)->type == 0)
							event_feature->lv2_event_unref(event_feature->callback_data, lv2_event_get(&iterator, NULL));
						else if (lv2_event_get(&iterator, NULL)->type == midi_event_id)
							size = ev->size;

						uint8_t* midi_data = (uint8_t *) lv2_event_get(&iterator, NULL) + 12;

						if (size == 3 && ((*midi_data & 0xf0) == 0x80))
						{
							nuclear::channel_t channel = 0x80 ^ (*midi_data);
							nuclear::note_t note = (*midi_data+1);
							nuclear::velocity_t velocity = (*midi_data+2) * velocity_scale;
							plugin->note_off(0, channel, note, velocity);
						}
						else if (size == 3 && ((*midi_data & 0xf0) == 0x90))
						{
							nuclear::channel_t channel = 0x90 ^ (*midi_data);
							nuclear::note_t note = (*midi_data+1);
							nuclear::velocity_t velocity = (*midi_data+2) * velocity_scale;
							plugin->note_on(0, channel, note, velocity);
						}
						else if (size == 3 && ((*midi_data & 0xf0) == 0xA0))
						{
							nuclear::channel_t channel = 0xA0 ^ (*midi_data);
							nuclear::note_t note = (*midi_data+1);
							nuclear::velocity_t velocity = (*midi_data+2) * velocity_scale;
							plugin->aftertouch(0, channel, note, velocity);
						}
						else if(size == 3 && ((*midi_data & 0xf0) == 0xB0))
						{
							//int channel = 0xB0 ^ (*midi_data);
							//int controller = (*midi_data+1);
							//int value = (*midi_data+2);
							//plugin->continous_controller(0, channel, controller, value);
						}
						else if(size == 2 && ((*midi_data & 0xf0) == 0xC0))
						{
							nuclear::channel_t channel = 0xC0 ^ (*midi_data);
							nuclear::uint32_t program = (*midi_data+1);
							plugin->program_change(0, channel, program);
						}
						else if(size == 2 && ((*midi_data & 0xf0) == 0xD0))
						{
							nuclear::channel_t channel = 0xD0 ^ (*midi_data);
							nuclear::velocity_t pressure = (*midi_data+1) * velocity_scale;
							plugin->channel_pressure(0, channel, pressure);
						}
						else if(size == 3 && ((*midi_data & 0xf0) == 0xE0))
						{
							//int channel = 0xE0 ^ (*midi_data);
							//int value_lo = (*midi_data+1);
							//int value_hi = (*midi_data+2);
							//nuclear::velocity_t value;
							//plugin->pitch_bend(0, channel, value);
						}					
					}
				}

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
		
