#ifndef DSP_H_
#define DSP_H_

// dsp.h
// Copyright (c) 2008 by Darren Landrum
//
// Contact: dmlandrum@gmail.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
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

class DSP
{
public:
	virtual DSP(int16_t buffer_size) = 0;
	virtual ~DSP() = 0;
	virtual sample_t* main_process() = 0;
};

} // !namespace nuclear

#endif /*DSP_H_*/
