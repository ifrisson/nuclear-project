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

#include "voice.h"

namespace nuclear
{

/////////////////////////////////////////////////////////////////////////////
// connection

connection::connection(node* node, const port_t port) :
	node(node),
	port(port)
{
}

void 
connection::process(const nframes_t nframes, sample_t* output, const tick_t tick)
{
	output = node->process(nframes, tick)[port];
}

/////////////////////////////////////////////////////////////////////////////
// input

void 
input::connect(node* node, const port_t output)
{
	if (output >= node->number_of_outputs())
		throw "No matching output!";
	
	connections.push_back(new connection(node, output));
}
	
void 
input::process(const nframes_t nframes, sample_t* output, const tick_t tick)
{
	sample_t** connection_buffers = allocate_buffers<sample_t>(connections.size(), nframes);
	for (port_t p = 0; p < connections.size(); ++p)
		connections[p]->process(nframes, connection_buffers[p], tick);
	mixdown<sample_t>(nframes, connection_buffers, output);
	free_buffers<sample_t>(connection_buffer);
}

/////////////////////////////////////////////////////////////////////////////
// node

node::node(block* block) :
	block(block),
	inputs(new input[block->number_of_inputs()]),
	internal_nframes(0),
	internal_tick(0)
{
}

node::~node() 
{
	delete [] inputs;
}

sample_t** 
node::process(const nframes_t nframes, const tick_t tick)
{
	if (internal_tick < tick)
	{
		internal_tick = tick;
		
		if (internal_nframes != nframes)
		{
			free_buffers<sample_t>(internal_buffers);
			internal_buffers = allocate_buffers<sample_t>(block->number_of_outputs(), nframes);
		}
		
		sample_t** input_buffers = allocate_buffers<sample_t>(block->number_of_inputs(), nframes);
		for (port_t p = 0; p < block->number_of_inputs(); ++p)
			inputs[p]->process(nframes, input_buffers[p], tick);
		block->compute(nframes, input_buffers, internal_buffers);
		free_buffers<sample_t>(input_buffers);
	}
	return internal_bufffers;
}

void 
node::connect(node* node, const port_t output, const port_t input)
{
	if (input >= block->number_of_inputs())
		throw "No matching input!";
	
	inputs[input]->connect(node, output);
}


/////////////////////////////////////////////////////////////////////////////
// voice_block

class voice_block :
	public block
{
public:
	voice_block(const port_t inputs, const port_t outputs) :
		ninputs(inputs),
		noutputs(outputs)
	{
	}

	void init(const uint_32)
	{
	}

	void set_buffers(sample_t** inputs, sample_t** outputs)
	{
		input_buffers = inputs;
		output_buffers = outputs;
	}

	void process(const nframes_t nframes, sample_t** inputs, sample_t** outputs)
	{
		for (port_t o = 0; o < noutputs; ++o)
		{
			nframes_t n = nframes;
			sample_t* src = input_buffers[o];
			sample_t* dst = outputs[o];
			while (n--) *dst++ += *src++;
		}

		for (port_t i = 0; i < ninputs; ++i)
		{
			nframes_t n = nframes;
			sample_t* src = inputs[i];
			sample_t* dst = output_buffers[i];
			while (n--) *dst++ += *src++;
		}
	}

	port_t number_of_inputs() { return ninputs; }
	port_t number_of_outputs() { return noutputs; }

private:
	const port_t ninputs;
	const port_t noutputs;
	sample_t** input_buffers;
	sample_t** output_buffers;
};

/////////////////////////////////////////////////////////////////////////////
// voice

voice::voice(const port_t ninputs, const port_t noutputs, const uint_32 srate) :
	io(new voice_block(ninputs, noutputs)),
	samplerate(srate),
	ticker(0)
{
	nodes["io"] = new node(io);
}

voice::~voice() 
{
}

void 
voice::process(const nframes_t nframes, sample_t** inputs, sample_t** outputs)
{
	io->set_buffers(inputs, outputs);
	nodes["io"]->process(nframes, ticker++);
}

void 
voice::create_node(const string_t node_id, block* block)
{
	block->init(samplerate);
	//fixme: check if node_id already exist as key
	nodes[node_id] = new node(block);
}

void 
voice::connect_node(const string_t from_node_id, const string_t to_node_id, const port_t output, const port_t input)
{
	//fixme: check if nodes_id's exist as keys
	nodes[to_node_id]->connect(nodes[from_node_id], output, input);
}

/////////////////////////////////////////////////////////////////////////////
// voice_director

void
voice_director::set_voice_builder(voice_builder* vb)
{
	builder = vb;
}

voice* 
voice_director::get_voice()
{
	return builder.get_voice();
}

void 
voice_director::construct_voice(const uint_32 srate)
{
	builder.create_new_voice(srate);
	builder.set_io();
	builder.create_nodes();
	builder.connect_nodes();
}

} // !namespace nuclear
