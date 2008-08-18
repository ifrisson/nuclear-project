#ifndef NUCLEAR_UTILITY_H
#define NUCLEAR_UTILITY_H

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

	template <typename T>
	void init_buffer(int nCols, T* buffer)
	{
		int n = nCols;
		T* dst = buffer;
		while (n--) *dst++ = 0;
	}

	template <typename T>
	void mix_buffer(int nCols, T* src_buffer, T* dst_buffer)
	{
		int n = nCols;
		T* src = src_buffer;
		T* dst = dst_buffer;
		while (n--) *dst++ += *src++;
	}

	template <typename T>
	void scale_buffer(int nCols, float scale, T* buffer)
	{
		int n = nCols;
		T* dst = buffer;
		while (n--) *dst++ *= scale;
	}

} // !namespace nuclear

#endif // !NUCLEAR_UTILITY_H
