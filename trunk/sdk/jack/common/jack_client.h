/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef NUCLEAR_JACK_CLIENT_H
#define NUCLEAR_JACK_CLIENT_H

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

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <jack/jack.h>
#include <jack/types.h>
#include <jack/midiport.h>
#include <jack/transport.h>

namespace nuclear
{

	namespace jack
	{

		int libjack_process_callback(jack_nframes_t, void*);
		void libjack_thread_init_callback(void*);
		void libjack_freewheel_callback(int, void*);
		int libjack_buffer_size_callback(jack_nframes_t, void*);
		int libjack_sample_rate_callback(jack_nframes_t, void*);
		int libjack_xrun_callback(void*);

		class client
		{
		public:
			client(const std::string client_name) :
				_processing(false)
			{
				const char* server_name = NULL;
				jack_options_t options = JackNullOption;
				jack_status_t status;
				
				_client = client_open(client_name.c_str(), options, &status, server_name);
				
				if (_client == NULL)
					throw  "Failed to open JACK client";
				
				jack_set_process_callback(_client, libjack_process_callback, this);
				jack_set_sample_rate_callback(_client, libjack_sample_rate_callback, this);
				jack_set_thread_init_callback(_client, libjack_thread_init_callback, this);
			}
			
			~client()
			{
				client_close(_client);
			}
			
			// ports
			void open_audio_in_ports(const int ports)
			{
				for (int i = 0; i < ports; ++i)
				{
					std::stringstream port_name;
					port_name << "in_" << i;
					
					jack_port_t* port = jack_port_register(_client, port_name.str().c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
					
					if (port == NULL)
						throw "No more JACK ports";
					
					_audio_in_ports.push_back(port);
				}
			}
			
			void close_audio_in_ports()
			{
				for (std::vector<jack_port_t*>::iterator i = _audio_in_ports.begin(); i != _audio_in_ports.end(); ++i)
					jack_port_unregister(_client, *i);
				
				_audio_in_ports.clear();
			}
			
			void open_audio_out_ports(const int ports)
			{
				for (int i = 0; i < ports; ++i)
				{
					std::stringstream port_name;
					port_name << "out_" << i;
					
					jack_port_t* port = jack_port_register(_client, port_name.str().c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
					
					if (port == NULL)
						throw "No more JACK ports";
					
					_audio_out_ports.push_back(port);
				}
			}
			
			void close_audio_out_ports()
			{
				for (std::vector<jack_port_t*>::iterator i = _audio_out_ports.begin(); i != _audio_out_ports.end(); ++i)
					jack_port_unregister(_client, *i);
				
				_audio_out_ports.clear();
			}
			
			void open_midi_in_ports(const int ports)
			{
				for (int i = 0; i < ports; ++i)
				{
					std::stringstream port_name;
					port_name << "in_" << i;
					
					jack_port_t* port = jack_port_register(_client, port_name.str().c_str(), JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
					
					if (port == NULL)
						throw "No more JACK ports";
					
					_midi_in_ports.push_back(port);
				}
			}
			
			void close_midi_in_ports()
			{
				for (std::vector<jack_port_t*>::iterator i = _midi_in_ports.begin(); i != _midi_in_ports.end(); ++i)
					jack_port_unregister(_client, *i);
				
				_midi_in_ports.clear();
			}
			
			void open_midi_out_ports(const int ports)
			{
				for (int i = 0; i < ports; ++i)
				{
					std::stringstream port_name;
					port_name << "out_" << i;
					
					jack_port_t* port = jack_port_register(_client, port_name.str().c_str(), JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
					
					if (port == NULL)
						throw "No more JACK ports";
					
					_midi_out_ports.push_back(port);
				}
			}
			
			void close_midi_out_ports()
			{
				for (std::vector<jack_port_t*>::iterator i = _midi_out_ports.begin(); i != _midi_out_ports.end(); ++i)
					jack_port_unregister(_client, *i);
				
				_midi_out_ports.clear();
			}
			
			// transport
			void transport_start()
			{
				jack_transport_start(_client); 
			}
			
			void transport_stop()
			{
				jack_transport_stop(_client); 
			}
			
			void transport_set_location(jack_nframes_t frame)
			{ 
				jack_transport_locate(_client, frame); 
			}
			
			jack_nframes_t transport_get_location()
			{
				jack_position_t pos;
				jack_transport_state_t state = jack_transport_query(_client, &pos);
				return pos.frame;
			}
			
			bool transport_rolling()
			{
				jack_position_t pos;
				jack_transport_state_t state = jack_transport_query(_client, &pos);
				return (state == JackTransportRolling);
			}
			
			bool transport_stopped()
			{
				jack_position_t pos;
				jack_transport_state_t state = jack_transport_query(_client, &pos);
				return (state == JackTransportStopped);
			}
			
			// MIDI events
			void send_note_off(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity)
			{
				if (_processing && port < _midi_out_ports.size())
				{
					void* buffer = jack_port_get_buffer(_midi_out_ports[port], buffer_size());
					jack_midi_data_t* event = jack_midi_event_reserve(buffer, 0, 3);
					event[0] = 0x80 | channel;
					event[1] = note;
					event[2] = velocity;
				}
			}
			
			void send_note_on(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity)
			{
				if (_processing && port < _midi_out_ports.size())
				{
					void* buffer = jack_port_get_buffer(_midi_out_ports[port], buffer_size());
					jack_midi_data_t* event = jack_midi_event_reserve(buffer, 0, 3);
					event[0] = 0x90 | channel;
					event[1] = note;
					event[2] = velocity;
				}
			}
			
			void send_aftertouch(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity)
			{
				if (_processing && port < _midi_out_ports.size())
				{
					void* buffer = jack_port_get_buffer(_midi_out_ports[port], buffer_size());
					jack_midi_data_t* event = jack_midi_event_reserve(buffer, 0, 3);
					event[0] = 0xA0 | channel;
					event[1] = note;
					event[2] = velocity;
				}
			}
			
			void send_continous_controller(int port, jack_midi_data_t channel, jack_midi_data_t controller, jack_midi_data_t value)
			{
				if (_processing && port < _midi_out_ports.size())
				{
					void* buffer = jack_port_get_buffer(_midi_out_ports[port], buffer_size());
					jack_midi_data_t* event = jack_midi_event_reserve(buffer, 0, 3);
					event[0] = 0xB0 | channel;
					event[1] = controller;
					event[2] = value;
				}
			}
			
			void send_patch_change(int port, jack_midi_data_t channel, jack_midi_data_t program)
			{
				if (_processing && port < _midi_out_ports.size())
				{
					void* buffer = jack_port_get_buffer(_midi_out_ports[port], buffer_size());
					jack_midi_data_t* event = jack_midi_event_reserve(buffer, 0, 2);
					event[0] = 0xC0 | channel;
					event[1] = program;
				}
			}
			
			void send_channel_pressure(int port, jack_midi_data_t channel, jack_midi_data_t pressure)
			{
				if (_processing && port < _midi_out_ports.size())
				{
					void* buffer = jack_port_get_buffer(_midi_out_ports[port], buffer_size());
					jack_midi_data_t* event = jack_midi_event_reserve(buffer, 0, 2);
					event[0] = 0xD0 | channel;
					event[1] = pressure;
				}
			}
			
			void send_pitch_bend(int port, jack_midi_data_t channel, jack_midi_data_t value_lo, jack_midi_data_t value_hi)
			{
				if (_processing && port < _midi_out_ports.size())
				{
					void* buffer = jack_port_get_buffer(_midi_out_ports[port], buffer_size());
					jack_midi_data_t* event = jack_midi_event_reserve(buffer, 0, 3);
					event[0] = 0xE0 | channel;
					event[1] = value_lo;
					event[2] = value_hi;
				}
			}
			
			// audio
			jack_default_audio_sample_t* get_audio_in_samples(const int port)
			{
				if (_processing && port < _audio_in_ports.size())
					return (jack_default_audio_sample_t *)jack_port_get_buffer(_audio_in_ports[port], buffer_size());
				else
					return NULL;
			}
			
			jack_default_audio_sample_t* get_audio_out_samples(const int port)
			{
				if (_processing && port < _audio_out_ports.size())
					return (jack_default_audio_sample_t *)jack_port_get_buffer(_audio_out_ports[port], buffer_size());
				else
					return NULL;
			}

			jack_nframes_t sample_rate()
			{
				return jack_get_sample_rate(_client);
			}
			
			jack_nframes_t buffer_size()
			{
				return jack_get_buffer_size(_client);
			}

			// client
			void activate()
			{
				if (jack_activate(_client))
					throw "Cannot activate JACK client";
			}
			
			void deactivate()
			{
				if (jack_deactivate(_client)) 
					throw "Cannot deactivate JACK client";
			}
			
			bool is_realtime()
			{
				return jack_is_realtime(_client) == 1 ? true : false;
			}
			
		protected:
			// callbacks
			virtual void on_process() = 0;
			virtual void on_thread_init() {}
			virtual void on_freewheel_mode_enter() {}
			virtual void on_freewheel_mode_exit() {}
			virtual void on_sample_rate_change() {}
			virtual void on_buffer_size_change() {}
			virtual void on_xrun() {}
			
			virtual void on_note_off(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) {}
			virtual void on_note_on(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) {}
			virtual void on_aftertouch(int port, jack_midi_data_t channel, jack_midi_data_t note, jack_midi_data_t velocity) {}
			virtual void on_continous_controller(int port, jack_midi_data_t channel, jack_midi_data_t controller, jack_midi_data_t value) {}
			virtual void on_patch_change(int port, jack_midi_data_t channel, jack_midi_data_t program) {}
			virtual void on_channel_pressure(int port, jack_midi_data_t channel, jack_midi_data_t pressure) {}
			virtual void on_pitch_bend(int port, jack_midi_data_t channel, jack_midi_data_t value_lo, jack_midi_data_t value_hi) {}
			
		private:
			friend int libjack_process_callback(jack_nframes_t, void*);
			friend void libjack_thread_init_callback(void*);
			friend void libjack_freewheel_callback(int, void*);
			friend int libjack_buffer_size_callback(jack_nframes_t, void*);
			friend int libjack_sample_rate_callback(jack_nframes_t, void*);
			friend int libjack_xrun_callback(void*);
			
			int process_callback(jack_nframes_t nframes)
			{
				_processing = true;
				
				for (std::vector<jack_port_t*>::iterator i = _midi_out_ports.begin(); i != _midi_out_ports.end(); ++i)
				{
					void* buffer = jack_port_get_buffer(*i, nframes);
			jack_midi_clear_buffer(buffer);
				}
				
				jack_midi_event_t event;
				for (int port = 0; port < _midi_in_ports.size(); ++port)
				{
					void* buffer = jack_port_get_buffer(_midi_in_ports[port], buffer_size());
					jack_nframes_t event_count = jack_midi_get_event_count(buffer);
					for (int event_index = 0; event_index < event_count; ++event_index)
					{
						jack_midi_event_get(&event, buffer, event_index);
						
						if ((event.buffer[0] & 0xf0) == 0x80)
						{
							int channel = 0x80 ^ event.buffer[0];
							int note = event.buffer[1];
							int velocity = event.buffer[2];
							on_note_off(port, channel, note, velocity);
						}
						else if((event.buffer[0] & 0xf0) == 0x90)
						{
							int channel = 0x90 ^ event.buffer[0];
							int note = event.buffer[1];
							int velocity = event.buffer[2];
							on_note_on(port, channel, note, velocity);
						}
						else if((event.buffer[0] & 0xf0) == 0xA0)
						{
							int channel = 0xA0 ^ event.buffer[0];
							int note = event.buffer[1];
							int velocity = event.buffer[2];
							on_aftertouch(port, channel, note, velocity);
						}
						else if((event.buffer[0] & 0xf0) == 0xB0)
						{
							int channel = 0xB0 ^ event.buffer[0];
							int controller = event.buffer[1];
							int value = event.buffer[2];
							on_continous_controller(port, channel, controller, value);
						}
						else if((event.buffer[0] & 0xf0) == 0xC0)
						{
							int channel = 0xC0 ^ event.buffer[0];
							int program = event.buffer[1];
							on_patch_change(port, channel, program);
						}
						else if((event.buffer[0] & 0xf0) == 0xD0)
						{
							int channel = 0xD0 ^ event.buffer[0];
							int pressure = event.buffer[1];
							on_channel_pressure(port, channel, pressure);
						}
						else if((event.buffer[0] & 0xf0) == 0xE0)
						{
							int channel = 0xE0 ^ event.buffer[0];
							int value_lo = event.buffer[1];
							int value_hi = event.buffer[2];
							on_pitch_bend(port, channel, value_lo, value_hi);
						}
					}
				}
				
				on_process();
				_processing = false;
				return 0;
			}
			
			void thread_init_callback()
			{
				on_thread_init();
			}
			
			void freewheel_callback(int starting)
			{
				if (starting != 0)
					on_freewheel_mode_enter();
				else
					on_freewheel_mode_exit();
			}
			
			int buffer_size_callback(jack_nframes_t nframes)
			{
				on_buffer_size_change();
				return 0;
			}
			
			int sample_rate_callback(jack_nframes_t nframes)
			{
				on_sample_rate_change();
				return 0;
			}
			
			int xrun_callback()
			{
				on_xrun();
		return 0;
			}
			
			std::vector<jack_port_t*> _audio_in_ports;
			std::vector<jack_port_t*> _audio_out_ports;
			std::vector<jack_port_t*> _midi_in_ports;
			std::vector<jack_port_t*> _midi_out_ports;
			
			client_t* _client;
			bool _processing;
		};

		int 
		libjack_process_callback(jack_nframes_t nframes, void* arg) 
		{
			return static_cast<nuclear::client*>(arg)->process_callback(nframes);
		}
		
		void
		libjack_thread_init_callback(void* arg)
		{
			static_cast<nuclear::client*>(arg)->thread_init_callback();
		}
		
		void 
		libjack_freewheel_callback(int starting, void* arg)
		{
			static_cast<nuclear::client*>(arg)->freewheel_callback(starting);
		}
		
		int
		libjack_buffer_size_callback(jack_nframes_t nframes, void* arg)
		{
			return static_cast<nuclear::client*>(arg)->buffer_size_callback(nframes);
}
		
		int 
		libjack_sample_rate_callback(jack_nframes_t nframes, void* arg)
		{
			return static_cast<nuclear::client*>(arg)->sample_rate_callback(nframes);
		}
		
		int 
		libjack_xrun_callback(void* arg)
		{
			static_cast<nuclear::client*>(arg)->xrun_callback();
		}

	} // !namespace jack

} // !namespace nuclear

#endif // !NUCLEAR_JACK_CLIENT_H
