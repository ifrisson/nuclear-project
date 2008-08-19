/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef NUCLEAR_ENGINE_H
#define NUCLEAR_ENGINE_H

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

	/// Baseclass for synths and effects
	class engine
	{
	public:
		engine() {}
		virtual ~engine() {}

		/// Initate engine
		virtual void init(uint32_t srate) = 0;
		/// Connect data to port
		virtual void connect_port(port_t port, void* data_location) = 0;
		/// Activate engine
		virtual void activate() = 0;
		/// Run engine
		virtual void run(nframes_t nframes) = 0;
		/// Deactivate engine
		virtual void deactivate() = 0;

		/// Return pointer to port value or buffer
		virtual void* p(nuclear::port_t port) = 0;
	};

} // !namespace nuclear

#endif // !NUCLEAR_ENGINE_H
