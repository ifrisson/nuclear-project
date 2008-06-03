// Vivace Performer
// Copyright (c) 2008, Anders Dahnielson
//
// Contact: anders@dahnielson.com

#include "commands.h"
#include "sequencer.h"
#include <iostream>
#include <lo/lo.h>

namespace 
{

void error(int num, const char *msg, const char *path)
{
	std::cerr << "liblo server error " << num << " in path " << path << ": " << msg << std::endl;
}

int 
seq_note_on(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->set_command(argv[0]->i, new NoteOnCommand(seq, argv[1]->i, argv[2]->i, argv[3]->i, argv[4]->i));
	return 0;
}

int 
seq_note_off(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->set_command(argv[0]->i, new NoteOffCommand(seq, argv[1]->i, argv[2]->i, argv[3]->i, argv[4]->i));
	return 0;
}

int 
seq_aftertouch(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->set_command(argv[0]->i, new AftertouchCommand(seq, argv[1]->i, argv[2]->i, argv[3]->i, argv[4]->i));
	return 0;
}

int 
seq_continous_controller(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->set_command(argv[0]->i, new ContinousControllerCommand(seq, argv[1]->i, argv[2]->i, argv[3]->i, argv[4]->i));
	return 0;
}

int 
seq_patch_change(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->set_command(argv[0]->i, new PatchChangeCommand(seq, argv[1]->i, argv[2]->i, argv[3]->i));
	return 0;
}

int 
seq_channel_pressure(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->set_command(argv[0]->i, new ChannelPressureCommand(seq, argv[1]->i, argv[2]->i, argv[3]->i));
	return 0;
}

int 
seq_pitch_bend(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->set_command(argv[0]->i, new NoteOnCommand(seq, argv[1]->i, argv[2]->i, argv[3]->i, argv[4]->i));
	return 0;
}

int 
seq_clear(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->clear();
	return 0;
}

int 
client_activate(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->activate();
	return 0;
}

int 
client_deactivate(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->deactivate();
	return 0;
}

int
client_open_ports(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->open_ports(argv[0]->i);
	return 0;
}

int
client_close_ports(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->close_ports();
	return 0;
}

int
transport_start(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->transport_start();
	return 0;
}

int
transport_stop(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->transport_stop();
	return 0;
}

int
transport_locate(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	Sequencer* seq = static_cast<Sequencer*>(user_data);
	seq->transport_set_location(argv[0]->i);
	return 0;
}

} // !namespace

int 
main(int argc, char **argv)
{
	Sequencer* seq = new Sequencer();

	lo_server_thread st = lo_server_thread_new("7770", error);
	lo_server_thread_add_method(st, "/seq/note_on", "iiiii", seq_note_on, seq);
	lo_server_thread_add_method(st, "/seq/note_off", "iiiii", seq_note_off, seq);
	lo_server_thread_add_method(st, "/seq/aftertouch", "iiiii", seq_aftertouch, seq);
	lo_server_thread_add_method(st, "/seq/continous_controller", "iiiii", seq_continous_controller, seq);
	lo_server_thread_add_method(st, "/seq/patch_change", "iiii", seq_patch_change, seq);
	lo_server_thread_add_method(st, "/seq/channel_pressure", "iiii", seq_channel_pressure, seq);
	lo_server_thread_add_method(st, "/seq/pitch_bend", "iiiii", seq_pitch_bend, seq);
	lo_server_thread_add_method(st, "/seq/clear", "", seq_clear, seq);
	lo_server_thread_add_method(st, "/client/activate", "", client_activate, seq);
	lo_server_thread_add_method(st, "/client/deactivate", "", client_deactivate, seq);
	lo_server_thread_add_method(st, "/client/open_ports", "i", client_open_ports, seq);
	lo_server_thread_add_method(st, "/client/close_ports", "", client_close_ports, seq);
	lo_server_thread_add_method(st, "/transport/start", "", transport_start, seq);
	lo_server_thread_add_method(st, "/transport/stop", "", transport_stop, seq);
	lo_server_thread_add_method(st, "/transport/locate", "i", transport_locate, seq);
	lo_server_thread_start(st);

	while (1)
		sleep(1);
}


