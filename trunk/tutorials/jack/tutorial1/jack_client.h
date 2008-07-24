#ifndef _JACK_CLIENT_H
#define _JACK_CLIENT_H

// Simple JACK Client
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
#include <vector>
#include <jack/types.h>
#include <jack/midiport.h>

int libjack_process_callback(jack_nframes_t, void*);

class JackClient
{
public:
	JackClient(const std::string client_name);
	~JackClient();

	// ports
	void open_audio_in_ports(const int ports);
	void close_audio_in_ports();
	void open_audio_out_ports(const int ports);
	void close_audio_out_ports();

	void open_midi_in_ports(const int ports);
	void close_midi_in_ports();
	void open_midi_out_ports(const int ports);
	void close_midi_out_ports();

	// transport
	void transport_start();
	void transport_stop();
	void transport_set_location(jack_nframes_t frame);
	jack_nframes_t transport_get_location();
	bool transport_rolling();
	bool transport_stopped();

	// MIDI events
	void send_note_off(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity);
	void send_note_on(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity);
	void send_aftertouch(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity);
	void send_continous_controller(int port, jack_midi_data_t channel, jack_midi_data_t controller, jack_midi_data_t value);
	void send_patch_change(int port, jack_midi_data_t channel, jack_midi_data_t program);
	void send_channel_pressure(int port, jack_midi_data_t channel, jack_midi_data_t pressure);
	void send_pitch_bend(int port, jack_midi_data_t channel, jack_midi_data_t value_lo, jack_midi_data_t value_hi);

	virtual void on_note_off(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) {}
	virtual void on_note_on(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) {}
	virtual void on_aftertouch(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) {}
	virtual void on_continous_controller(int port, jack_midi_data_t channel, jack_midi_data_t controller, jack_midi_data_t value) {}
	virtual void on_patch_change(int port, jack_midi_data_t channel, jack_midi_data_t program) {}
	virtual void on_channel_pressure(int port, jack_midi_data_t channel, jack_midi_data_t pressure) {}
	virtual void on_pitch_bend(int port, jack_midi_data_t channel, jack_midi_data_t value_lo, jack_midi_data_t value_hi) {}

	// audio
	float* get_audio_in_samples(const int port);
	float* get_audio_out_samples(const int port);

	jack_nframes_t sample_rate();
	jack_nframes_t buffer_size();

	virtual void on_process() = 0;

	// client
	void activate();
	void deactivate();

private:
	friend int libjack_process_callback(jack_nframes_t, void*);
	int process_callback(jack_nframes_t nframes);

	std::vector<jack_port_t*> _audio_in_ports;
	std::vector<jack_port_t*> _audio_out_ports;
	std::vector<jack_port_t*> _midi_in_ports;
	std::vector<jack_port_t*> _midi_out_ports;

	jack_client_t* _client;
	bool _processing;
};

#endif // !_JACK_CLIENT_H
