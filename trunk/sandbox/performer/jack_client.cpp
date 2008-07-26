// Vivace Performer
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

#include "jack_client.h"
#include <iostream>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <jack/transport.h>

namespace
{

int 
libjack_process_callback(jack_nframes_t nframes, void* arg) 
{
        return static_cast<JackClient*>(arg)->process_callback(nframes);
}

} // !namespace

JackClient::JackClient(const std::string client_name)
{
	const char* server_name = NULL;
	jack_options_t options = JackNullOption;
	jack_status_t status;

	client = jack_client_open(client_name.c_str(), options, &status, server_name);
	
	if (client == NULL)
		throw  "Failed to open JACK client";
	
	jack_set_process_callback(client, libjack_process_callback, this);
}

JackClient::~JackClient()
{
	jack_client_close(client);
}

void 
JackClient::open_ports(const int outputs)
{
	for (int i = 0; i < outputs; ++i) 
	{
		char buf[256];
		snprintf(buf, 256, "out_%d", i);
		output_port[i] = jack_port_register(client, buf, JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
		if (output_port[i] == NULL)
			throw "No more JACK ports available";
		ports++;
	}
}

void 
JackClient::close_ports()
{
	for (int i = 0; i < ports; ++i)
		jack_port_unregister(client, output_port[i]);	
}

jack_nframes_t 
JackClient::sample_rate()
{
	return jack_get_sample_rate(client);
}

void 
JackClient::transport_start() 
{
	jack_transport_start(client); 
}

void 
JackClient::transport_stop() 
{
	jack_transport_stop(client); 
}

void 
JackClient::transport_set_location(jack_nframes_t frame) 
{ 
	jack_transport_locate(client, frame); 
}

jack_nframes_t 
JackClient::transport_get_location()
{
	jack_position_t pos;
	jack_transport_state_t state = jack_transport_query(client, &pos);
	return pos.frame;
}

bool 
JackClient::transport_rolling()
{
	jack_position_t pos;
	jack_transport_state_t state = jack_transport_query(client, &pos);
	return (state == JackTransportRolling);
}

bool 
JackClient::transport_stopped()
{
	jack_position_t pos;
	jack_transport_state_t state = jack_transport_query(client, &pos);
	return (state == JackTransportStopped);
}

void 
JackClient::note_off(int port, int channel, int note, int velocity)
{
	jack_midi_data_t* buffer = jack_midi_event_reserve(port_buf[port], 0, 3);
	buffer[0] = 0x80 | channel;
	buffer[1] = note;
	buffer[2] = velocity;
}

void 
JackClient::note_on(int port, int channel, int note, int velocity)
{
	jack_midi_data_t* buffer = jack_midi_event_reserve(port_buf[port], 0, 3);
	buffer[0] = 0x90 | channel;
	buffer[1] = note;
	buffer[2] = velocity;
}

void 
JackClient::aftertouch(int port, int channel, int note, int velocity)
{
	jack_midi_data_t* buffer = jack_midi_event_reserve(port_buf[port], 0, 3);
	buffer[0] = 0xA0 | channel;
	buffer[1] = note;
	buffer[2] = velocity;
}

void 
JackClient::continous_controller(int port, int channel, int controller, int value)
{
	jack_midi_data_t* buffer = jack_midi_event_reserve(port_buf[port], 0, 3);
	buffer[0] = 0xB0 | channel;
	buffer[1] = controller;
	buffer[2] = value;
}

void 
JackClient::patch_change(int port, int channel, int program)
{
	jack_midi_data_t* buffer = jack_midi_event_reserve(port_buf[port], 0, 2);
	buffer[0] = 0xC0 | channel;
	buffer[1] = program;
}

void 
JackClient::channel_pressure(int port, int channel, int pressure)
{
	jack_midi_data_t* buffer = jack_midi_event_reserve(port_buf[port], 0, 2);
	buffer[0] = 0xD0 | channel;
	buffer[1] = pressure;
}

void 
JackClient::pitch_bend(int port, int channel, int value_lo, int value_hi)
{
	jack_midi_data_t* buffer = jack_midi_event_reserve(port_buf[port], 0, 3);
	buffer[0] = 0xE0 | channel;
	buffer[1] = value_lo;
	buffer[2] = value_hi;
}

int 
JackClient::process_callback(jack_nframes_t nframes)
{
	for (int port = 0; port < ports; ++port)
	{
		port_buf[port] = jack_port_get_buffer(output_port[port], nframes);
		jack_midi_clear_buffer(port_buf[port]);
	}
	
	process(nframes);
	
	return 0;
}

void 
JackClient::activate()
{
	if (jack_activate(client))
		throw "Cannot activate JACK client";
}

void
JackClient::deactivate()
{
	if (jack_deactivate(client)) 
		throw "Cannot deactivate JACK client";
}
