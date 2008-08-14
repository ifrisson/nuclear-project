#ifndef NUCLEAR_OSCUI_H
#define NUCLEAR_OSCUI_H

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

#include "paramui.h"
#include <string>
#include <iostream>

extern "C" {
#include <lo/lo.h>
}

namespace nuclear
{

void liblo_error_callback(int, const char*, const char*);
int liblo_method_callback(const char*, const char*, lo_arg**, int, void*, void*);

class oscui :
	public paramui
{
public:
	oscui(std::string port) :
		paramui()
	{
		lo_server_thread _thread = lo_server_thread_new(port.c_str(), error);
		lo_server_thread_add_method(_thread, NULL, "f", liblo_callback, this);
		lo_server_thread_start(_thread);
	}

	~oscui()
	{
		lo_server_thread_free(_thread);
	}

private:
	friend void liblo_error_callback(int, const char*, const char*);
	friend int liblo_method_callback(const char*, const char*, lo_arg**, int, void*, void*);

	lo_server_thread _thread;
};

void 
liblo_error_callback(int num, const char *msg, const char *path)
{
	std::cerr << "liblo server error " << num << " in path " << path << ": " << msg << std::endl;
}

int 
liblo_method_callback(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	static_cast<nuclear::oscui*>(user_data)->set_option(path, argv[0]->f);
	return 0;
}

} // !namespace nuclear

#endif // !NUCLEAR_OSCUI_H
