#ifndef GRAPH_VOICE_H
#define GRAPH_VOICE_H

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

#include "block.h"
#include "types.h"

namespace nuclear
{

// Forward declaration
class node;

/////////////////////////////////////////////////////////////////////////////
// connection

class connection
{
public:
	connection(node* node, const port_t port);
	void process(const nframes_t nframes, sample_t* output, const tick_t tick);

private:
	node* node;
	const port_t port;
};

/////////////////////////////////////////////////////////////////////////////
// input

class input
{
public:
	input()	{}
	~input() {}
	void connect(node* node, const port_t output);
	void process(const nframes_t nframes, sample_t* output, const tick_t tick);

private:
	std::vector<connection*> connections;
};

/////////////////////////////////////////////////////////////////////////////
// node

class node
{
public:
	node(block* block);
	~node();

	const port_t number_of_inputs() { return block->number_of_inputs(); }
	const port_t number_of_outputs() { return block->number_of_outputs(); }

	sample_t** process(const nframes_t nframes, const tick_t tick);
	void connect(node* node, const port_t output, const port_t input);

private:
	block* block;
	input* inputs;
	nframes_t internal_nframes;
	tick_t internal_tick;
	sample_t** internal_buffers;
};

/////////////////////////////////////////////////////////////////////////////
// voice

class voice
{
public:
	voice(const port_t ninputs, const port_t noutputs);
	~voice() {}
	void process(const nframes_t nframes, sample_t** inputs, sample_t** outputs);
	void create_node(const string_t node_id, block* block);
	void connect_node(const string_t from_node_id, const string_t to_node_id, const port_t output, const port_t input);
	
private:
	block* io;
	tick_t ticker;
	std::map<string_t, node*> nodes;
};

/////////////////////////////////////////////////////////////////////////////
// voice_builder

class voice_builder
{
public:
	voice* get_voice()
	{
		return voice;
	}

	void create_new_voice(const uint_32 srate)
	{
		voice = new voice(number_of_inputs(), number_of_outputs(), srate);
	}

	virtual void create_nodes() = 0;
	virtual void connect_nodes() = 0;

	virtual port_t number_of_inputs() = 0;
	virtual port_t number_of_outputs() = 0;

protected:
	voice* voice;
};

/////////////////////////////////////////////////////////////////////////////
// voice_director

class voice_director
{
public:
	void set_voice_builder(voice_builder* vb);
	voice* get_voice();
	void construct_voice(const uint_32 srate);

private: 
	voice_builder* builder;
};

} // !namespace nuclear

#endif // !GRAPH_VOICE_H
