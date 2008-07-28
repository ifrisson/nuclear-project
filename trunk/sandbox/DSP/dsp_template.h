#ifndef DSPTEMPLATE_H_
#define DSPTEMPLATE_H_

// dsp_template.h
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

#include "dsp.h"

namespace nuclear
{

class DSPTemplate: public DSP
{
public:
	DSPTemplate(int16_t buffer_size)
	{
		sample_t* output_buffer = new sample_t[buffer_size];
		// any space for delay lines and the like should be defined here
	}
	
	~DSPTemplate()
	{
		delete [] output_buffer;
		// likewise, space for delay lines and the like should be deleted here
	}
	
	sample_t* main_process(/* arguments go here */)
	{
		// main code goes here
		
		return output_buffer; // return pointer for the newly-processed buffer
	}
	
protected:
	// any extra functions needed can be defined from this point on
	
private:
	sample_t* output_buffer;
	// set any defined extra space to private here
};

} // !namespace nuclear

#endif /*DSPTEMPLATE_H_*/