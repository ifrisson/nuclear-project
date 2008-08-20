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
#include "lv2_mapper.h"

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
			static mapper* map;

			// Event extension
			static LV2_Event_Buffer* event_buffer;
			static LV2_Event_Feature* event_feature;

			// MIDI extension
			static int midi_event_id;

			// URI-Map extension
			static LV2_URI_Map_Feature* map_feature;

			// Ports
			static std::vector<void*> control_ports;
			static std::vector<nuclear::sample_t*> audio_ports;
			static std::vector<LV2_Event_Buffer*> midi_ports;

		public:
			plugin(mapper* mapping)
			{
				plugin::map = mapping;
				g_lv2_descriptor = new LV2_Descriptor;
				g_lv2_descriptor->URI = plugin::map->uri().c_str();
				g_lv2_descriptor->instantiate = plugin::instantiate;
				g_lv2_descriptor->connect_port = plugin::connect_port;
				g_lv2_descriptor->activate = plugin::activate;
				g_lv2_descriptor->run = plugin::run;
				g_lv2_descriptor->deactivate = plugin::deactivate;
				g_lv2_descriptor->cleanup = plugin::cleanup;
				g_lv2_descriptor->extension_data = plugin::extension_data;
			}
			
			static LV2_Handle instantiate(const struct _LV2_Descriptor* descriptor, double sample_rate, const char* bundle_path, const LV2_Feature* const* features)
			{
				nuclear::engine* engine = new T;

				// Look for features
				for(int i = 0; features[i]; ++i)
				{
					if (nuclear::string_t(features[i]->URI) == nuclear::string_t(LV2_URI_MAP_URI))
					{
						plugin::map_feature = features[i]->data;
						plugin::midi_event_id = plugin::map_feature->uri_to_id(plugin::map_feature->callback_data, LV2_EVENT_URI, "http://lv2plug.in/ns/ext/midi#MidiEvent");
					}
					else if (nuclear::string_t(features[i]->URI) == nuclear::string_t(LV2_EVENT_URI))
					{
						plugin::event_feature = features[i]->data;
					}
				}

				// Do we require MIDI?
				if (dynamic_cast<nuclear::midi*>(engine))
				{
					if (plugin::midi_event_id == 0 || plugin::event_feature == NULL)
					{
						delete engine;
						return NULL;
					}
				}

				engine->init(sample_rate);
				return engine;
			}
			
			static void connect_port(LV2_Handle instance, uint32_t port, void* data_location)
			{
				switch (plugin::map->port_type(port))
				{
				case CONTROL:
					plugin::midi_ports.push_back(data_location);
					break;
				case AUDIO:
					plugin::audio_ports.push_back(static_cast<nuclear::sample_t*>(data_location));
					break;
				case MIDI:
					plugin::control_ports.push_back(static_cast<LV2_Event_Buffer*>(data_location));
					break;
				default:
					break;
				}
			}
			
			static void activate(LV2_Handle instance)
			{
				nuclear::engine* engine = static_cast<nuclear::engine*>(instance);
				engine->activate();
			}

			static void run_midi(nuclear::midi* engine, uint32_t sample_count)
			{
				const nuclear::float_t velocity_scale = 1.0f / 127;
				int port = 0;
				for (std::vector<LV2_Event_Buffer*>::iterator p = plugin::midi_ports.begin(); p != plugin::midi_ports.end(); ++p)
				{
					LV2_Event_Iterator iterator;
					for(lv2_event_begin(&iterator, (*p)); lv2_event_is_valid(&iterator); lv2_event_increment(&iterator))
					{
						uint16_t size = 0;
						if (lv2_event_get(&iterator, NULL)->type == 0)
							plugin::event_feature->lv2_event_unref(plugin::event_feature->callback_data, lv2_event_get(&iterator, NULL));
						else if (lv2_event_get(&iterator, NULL)->type == plugin::midi_event_id)
							size = lv2_event_get(&iterator, NULL)->size;

						uint8_t* midi_data = (uint8_t *) lv2_event_get(&iterator, NULL) + 12;

						if (size == 3 && ((*midi_data & 0xf0) == 0x80))
						{
							nuclear::channel_t channel = 0x80 ^ (*midi_data);
							nuclear::note_t note = (*midi_data+1);
							nuclear::velocity_t velocity = (*midi_data+2) * velocity_scale;
							engine->note_off(port, channel, note, velocity);
						}
						else if (size == 3 && ((*midi_data & 0xf0) == 0x90))
						{
							nuclear::channel_t channel = 0x90 ^ (*midi_data);
							nuclear::note_t note = (*midi_data+1);
							nuclear::velocity_t velocity = (*midi_data+2) * velocity_scale;
							engine->note_on(port, channel, note, velocity);
						}
						else if (size == 3 && ((*midi_data & 0xf0) == 0xA0))
						{
							nuclear::channel_t channel = 0xA0 ^ (*midi_data);
							nuclear::note_t note = (*midi_data+1);
							nuclear::velocity_t velocity = (*midi_data+2) * velocity_scale;
							engine->aftertouch(port, channel, note, velocity);
						}
						else if(size == 3 && ((*midi_data & 0xf0) == 0xB0))
						{
							//int channel = 0xB0 ^ (*midi_data);
							//int controller = (*midi_data+1);
							//int value = (*midi_data+2);
							//engine->continous_controller(port, channel, controller, value);
						}
						else if(size == 2 && ((*midi_data & 0xf0) == 0xC0))
						{
							nuclear::channel_t channel = 0xC0 ^ (*midi_data);
							nuclear::uint32_t program = (*midi_data+1);
							engine->program_change(port, channel, program);
						}
						else if(size == 2 && ((*midi_data & 0xf0) == 0xD0))
						{
							nuclear::channel_t channel = 0xD0 ^ (*midi_data);
							nuclear::velocity_t pressure = (*midi_data+1) * velocity_scale;
							engine->channel_pressure(port, channel, pressure);
						}
						else if(size == 3 && ((*midi_data & 0xf0) == 0xE0))
						{
							//int channel = 0xE0 ^ (*midi_data);
							//int value_lo = (*midi_data+1);
							//int value_hi = (*midi_data+2);
							//nuclear::velocity_t value;
							//engine->pitch_bend(port, channel, value);
						}
					}
					port++;
				}
			}

			static void run(LV2_Handle instance, uint32_t sample_count)
			{
				nuclear::engine* engine = static_cast<nuclear::engine*>(instance);

				nuclear::midi* midi = dynamic_cast<nuclear::midi*>(engine);
				if (midi) plugin::run_midi(midi, sample_count);

				engine->run(sample_count);
			}
		
			static void deactivate(LV2_Handle instance)
			{
				nuclear::engine* engine = static_cast<nuclear::engine*>(instance);
				engine->deactivate();
			}
		
			static void cleanup(LV2_Handle instance)
			{
				nuclear::engine* engine = static_cast<nuclear::engine*>(instance);
				delete engine;
			}
		
			static const void* extension_data(const char* uri)
			{
				return NULL;
			}
		};

	} // !namespace lv2
		
} // !namespace nuclear

/// Macro to register engine as LV2 plugin		
#define NUCLEAR_LV2_PLUGIN(T, MAP) static void* _ = new nuclear::lv2::plugin<T>(MAP);
		
#endif // !NUCLEAR_LV2_PLUGIN_H
