#ifndef DIGRAPH_H_
#define DIGRAPH_H_

// digraph.h
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
#include "node.h"

namespace nuclear
{

class Digraph
{
public:
	Digraph(int8_t nodes, int8_t size)
	{
		DSP* graph = new DSP[nodes];
		sample_t* output_buffer = new sample_t[size];
		buffer_size = size;
	}
	
	~Digraph()
	{
		delete graph;
		delete output_buffer;
	}
	
	void add_node(DSP* node_pointer)
	{
		
	}
	
	void del_node(DSP* node_pointer)
	{
		
	}
	
	void connect_node(DSP* receiver, port_t input_port, DSP* sender, port_t output_port)
	{
		
	}
	
	void disconnect_node(DSP* receiver, port_t input_port)
	{
		
	}
	
	sample_t* run_graph()
	{
		
	}
	
private:
	DSP* graph;
	sample_t* output_buffer;
	int8_t buffer_size;
};

} // !namespace nuclear

#endif /*DIGRAPH_H_*/
