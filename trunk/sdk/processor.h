#ifndef NUCLEAR_PROCESSOR_H
#define NUCLEAR_PROCESSOR_H

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

namespace nuclear
{

class processor
{
public:
        processor() {}
        virtual ~processor() {}
	
        virtual int getNumInputs() = 0;
        virtual int getNumOutputs() = 0;
        virtual void init(int srate) = 0;
        virtual void compute(int nframes, float** inputs, float** outputs)  = 0;
};

} // !namespace nuclear

#endif // !NUCLEAR_PROCESSOR_H
