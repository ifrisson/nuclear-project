#ifndef NUCLEAR_MIDI_H
#define NUCLEAR_MIDI_H

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

#include "types.h"

namespace nuclear
{

/// Abstract class inherited by engines that will handle MIDI events
class midi
{
public:
        midi() {}
        virtual ~midi() {}
	
	/////////////////////////////////////////////////////////
	// Channel Voice Messages

	/// Note off handler
	virtual void note_off(port_t port, channel_t channel, note_t note, velocity_t velocity) = 0;
	/// Note on handler
	virtual void note_on(port_t port, channel_t channel, note_t note, velocity_t velocity) = 0;
	/// Aftertouch handler
	virtual void aftertouch(port_t port, channel_t channel, note_t note, velocity_t velocity) = 0;
	/// Program change handler
	virtual void program_change(port_t port, channel_t channel, uint32_t program) = 0;
	/// Channel pressure handler
	virtual void channel_pressure(port_t port, channel_t channel, velocity_t pressure) = 0;
	/// Pitch bend handler
	virtual void pitch_bend(port_t port, channel_t channel, velocity_t value) = 0;

	/////////////////////////////////////////////////////////
	// Channel Mode Messages

	// High resolution continuous controllers

	/// Bank change handler
	virtual void bank_select(port_t port, channel_t channel, uint32_t bank) = 0;
	/// Modulation wheel handler
	virtual void modulation_wheel(port_t port, channel_t channel, uint32_t value) = 0;
	/// Breath controller handler
	virtual void breath_controller(port_t port, channel_t channel, uint32_t value) = 0;
	/// Foot controller handler
	virtual void foot_controller(port_t port, channel_t channel, uint32_t value) = 0;
	/// Portamento time handler
	virtual void portamento_time(port_t port, channel_t channel, uint32_t value) = 0;
	/// Data entry handler
	virtual void data_enry(port_t port, channel_t channel, uint32_t value) = 0;
	/// Channel volume handler
	virtual void channel_volume(port_t port, channel_t channel, uint32_t value) = 0;
	/// Balance handler
	virtual void balance(port_t port, channel_t channel, uint32_t value) = 0;
	/// Pan handler
	virtual void pan(port_t port, channel_t channel, uint32_t value) = 0;
	/// Expression controller handler
	virtual void expression_controller(port_t port, channel_t channel, uint32_t value) = 0;
	/// Effect control 1 handler
	virtual void effect_control_1(port_t port, channel_t channel, uint32_t value) = 0;
	/// Effect control 2 handler
	virtual void effect_control_2(port_t port, channel_t channel, uint32_t value) = 0;
	/// General purpose controller 1 handler
	virtual void general_purpose_controller_1(port_t port, channel_t channel, uint32_t value) = 0;
	/// General purpose controller 2 handler
	virtual void general_purpose_controller_2(port_t port, channel_t channel, uint32_t value) = 0;
	/// General purpose controller 3 handler
	virtual void general_purpose_controller_3(port_t port, channel_t channel, uint32_t value) = 0;
	/// General purpose controller 4 handler
	virtual void general_purpose_controller_4(port_t port, channel_t channel, uint32_t value) = 0;

	// Switches

	/// Sustain on/off handler
	virtual void sustain(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Portament on/off
	virtual void portamento(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Sostenuto handler
	virtual void sostenuto(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Soft pedal handler
	virtual void soft_pedal(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Legato handler
	virtual void legato(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Hold 2 handler
	virtual void hold_2(port_t port, channel_t channel, uint32_t value) = 0;	

	// Low resolution continous controllers

	/// Sound controller 1 handler
	virtual void sound_controller_1(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Sound controller 2 handler
	virtual void sound_controller_2(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Sound controller 3 handler
	virtual void sound_controller_3(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Sound controller 4 handler
	virtual void sound_controller_4(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Sound controller 5 handler
	virtual void sound_controller_5(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Sound controller 6 handler
	virtual void sound_controller_6(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Sound controller 7 handler
	virtual void sound_controller_7(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Sound controller 8 handler
	virtual void sound_controller_8(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Sound controller 9 handler
	virtual void sound_controller_9(port_t port, channel_t channel, uint32_t value) = 0;	
	/// Sound controller 10 handler
	virtual void sound_controller_10(port_t port, channel_t channel, uint32_t value) = 0;	
	/// General purpose controller 5 handler
	virtual void general_purpose_controller_5(port_t port, channel_t channel, uint32_t value) = 0;
	/// General purpose controller 6 handler
	virtual void general_purpose_controller_6(port_t port, channel_t channel, uint32_t value) = 0;
	/// General purpose controller 7 handler
	virtual void general_purpose_controller_7(port_t port, channel_t channel, uint32_t value) = 0;
	/// General purpose controller 8 handler
	virtual void general_purpose_controller_8(port_t port, channel_t channel, uint32_t value) = 0;
	/// Portamento controll handler
	virtual void portamento_controll(port_t port, channel_t channel, uint32_t value) = 0;
	/// Effects 1 handler
	virtual void effects_1(port_t port, channel_t channel, uint32_t value) = 0;
	/// Effects 2 handler
	virtual void effects_2(port_t port, channel_t channel, uint32_t value) = 0;
	/// Effects 3 handler
	virtual void effects_3(port_t port, channel_t channel, uint32_t value) = 0;
	/// Effects 4 handler
	virtual void effects_4(port_t port, channel_t channel, uint32_t value) = 0;
	/// Effects 5 handler
	virtual void effects_5(port_t port, channel_t channel, uint32_t value) = 0;

	/// All sound off handler
	virtual void all_sound_off(port_t port, channel_t channel) = 0;
	/// Reset all conrollers handler
	virtual void reset_all_controllers(port_t port, channel_t channel) = 0;
	/// Local control on/off handler
	virtual void local_control(port_t port, channel_t channel) = 0;
	/// All notes off handler
	virtual void all_notes_off(port_t port, channel_t channel) = 0;
	/// Omni mode off handler
	virtual void omni_mode_off(port_t port, channel_t channel) = 0;
	/// Omni mode on handler
	virtual void omni_mode_on(port_t port, channel_t channel) = 0;
	/// Mono mode on handler
	virtual void mono_mode_on(port_t port, channel_t channel) = 0;
	/// Poly mode on handler
	virtual void poly_mode_on(port_t port, channel_t channel) = 0;
};

} // !namespace nuclear

#endif // !NUCLEAR_MIDI_H
