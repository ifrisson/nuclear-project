/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef NUCLEAR_LADSPA_DESCRIPTOR_BUILDER_H
#define NUCLEAR_LADSPA_DESCRIPTOR_BUILDER_H

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
#include <vector>
#include "../../types.h"

namespace nuclear
{

	namespace ladspa
	{
		
		class descriptor_builder
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
		
	} // !namespace ladspa
	
} // !namespace nuclear

#endif // !NUCLEAR_LADSPA_DESCRIPTOR_BUILDER_H
