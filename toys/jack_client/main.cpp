// Simple JACK Client Demo
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

class JackClientDemo :
	public JackClient
{
public:
	JackClientDemo() :
		JackClient("jack_client_demo")
	{
		open_audio_out_ports(1);
		open_midi_in_ports(1);
	}

	~JackClientDemo()
	{
		close_audio_out_ports();
		close_midi_in_ports();
	}

	void on_process()
	{
	}
};


int 
main(int argc, char **argv)
{
	JackClientDemo* client = new JackClientDemo();
	client->activate();

	while (1)
		sleep(1);
}
