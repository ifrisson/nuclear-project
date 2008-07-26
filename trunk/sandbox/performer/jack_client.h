#ifndef PERFORMER_JACK_CLIENT_H
#define PERFORMER_JACK_CLIENT_H

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

#include <string>
#include <jack/types.h>

class JackClient
{
public:
	JackClient(const std::string client_name);
	~JackClient();

	void open_ports(const int outputs);
	void close_ports();

	jack_nframes_t sample_rate();

	// transport
	void transport_start();
	void transport_stop();
	void transport_set_location(jack_nframes_t frame);
	jack_nframes_t transport_get_location();
	bool transport_rolling();
	bool transport_stopped();

	// MIDI events
	void note_off(int port, int channel, int note, int velocity);
	void note_on(int port, int channel, int note, int velocity);
	void aftertouch(int port, int channel, int note, int velocity);
	void continous_controller(int port, int channel, int controller, int value);
	void patch_change(int port, int channel, int program);
	void channel_pressure(int port, int channel, int pressure);
	void pitch_bend(int port, int channel, int value_lo, int value_hi);

	virtual void process(jack_nframes_t nframes) = 0;
	int process_callback(jack_nframes_t nframes);

	// client
	void activate();
	void deactivate();

protected:
	int ports;
	void* port_buf[256];
	jack_port_t* output_port[256];
	jack_client_t* client;
};

#endif // !PERFORMER_JACK_CLIENT_H
