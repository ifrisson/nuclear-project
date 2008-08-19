/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef NUCLEAR_TYPES_H
#define NUCLEAR_TYPES_H

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

#include <boost/cstdint.hpp>
#include <string>

namespace nuclear
{

	typedef bool bool_t;

	typedef boost::int8_t int8_t;
	typedef boost::int16_t int16_t;
	typedef boost::int32_t int32_t;
	typedef boost::int64_t int64_t;

	typedef boost::uint8_t uint8_t;
	typedef boost::uint16_t uint16_t;
	typedef boost::uint32_t uint32_t;
	typedef boost::uint64_t uint64_t;

	typedef float float_t;
	typedef double double_t;

	typedef std::string string_t;

	// Audio

	/// Number of frames
	typedef boost::uint32_t nframes_t;
	/// Ticks
	typedef boost::uint64_t tick_t;
	/// 32-bit float samples
	typedef float sample_t;

	// MIDI

	/// Port
	typedef boost::uint32_t port_t;
	/// Channel value between 0...15
	typedef boost::uint32_t channel_t;
	/// Note value between 0...127
	typedef boost::uint32_t note_t;
	/// Velocity value between 0.0f...1.0f
	typedef float velocity_t;

} // !namespace nuclear

#endif // !NUCLEAR_TYPES_H
