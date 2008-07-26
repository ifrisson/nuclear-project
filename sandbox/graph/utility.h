#ifndef GRAPH_UTILITY_H
#define GRAPH_UTILITY_H

// Graph Demo
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

template <typename T>
void mixdown(const nframes_t nframes, const port_t inputs, T** input_buffers, T* output_buffer)
{
	for (port_t i = 0; i < inputs; ++i)
	{
		nframes_t n = nframes;
		T* src = input_buffers[i];
		T* dst = output_buffer;
		while (n--) *dst++ += *src++;
	}
}

template <typename T>
T** allocate_buffers(int nRows, int nCols)
{
	T **ppi = new T*[nRows];
	T *curPtr = new T[nRows * nCols];
	for(int i = 0; i < nRows; ++i)
	{
		*(ppi + i) = curPtr;
		curPtr += nCols;
	}
	return ppi;
}
	
template <typename T>
void free_buffers(T** buffers)
{
	delete [] *buffers;
	delete [] buffers;
}

} // !namespace nuclear

#endif // !GRAPH_ENGINE_H
