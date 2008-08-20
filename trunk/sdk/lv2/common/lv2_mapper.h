/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef NUCLEAR_LV2_MAPPER_H
#define NUCLEAR_LV2_MAPPER_H

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

#include "../../types.h"

namespace nuclear
{

	namespace lv2
	{

		enum port_type_t
		{
			CONTROL = 1,
			AUDIO,
			MIDI
		}

		class mapper
		{
		public:
			virtual nuclear::string_t uri() = 0;
			virtual port_type_t port_type(nuclear::uint32_t index) = 0;
		};

	} // !namespace lv2

} // !namespace nuclear

#endif // !NUCLEAR_LV2_MAPPER_H
