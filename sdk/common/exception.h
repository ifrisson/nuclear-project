#ifndef NUCLEAR_EXCEPTION_H
#define NUCLEAR_EXCEPTION_H

// Nuclear
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

#include <exception>
#include "../types.h"

namespace nuclear
{

class Exception : 
	public std::exception
{
public:
	Exception(string_t msg)
	{
		error = msg;
	}
	
	~Exception() throw() {}
	
	virtual const char* what() const throw()
	{
		return error.c_str();
	}

private:
	string_t error;
};

} // !namespace nuclear

#endif // !NUCLEAR_EXCEPTION_H
