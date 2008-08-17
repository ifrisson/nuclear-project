#ifndef NUCLEAR_SYNTH_DESCRIPTOR_H
#define NUCLEAR_SYNTH_DESCRIPTOR_H

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

#include "types.h"

namespace nuclear
{

/// Abstract descriptor
class synth_descriptor
{
public:
	void set_label(string_t label);
	void set_name(string_t name);
	void set_maker(string_t maker);
	void set_copyright(string_t copyright);
	void add_control(string_t name, float_t min, float_t max);
	void add_input(string_t name);
	void add_output(string_t name);
};

} // !namespace nuclear

#endif // !NUCLEAR_SYNTH_DESCRIPTOR_H
