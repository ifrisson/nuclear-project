////////////////////////////////////////////////////////////////////////////////
// Channel EQ
// by Darren Landrum, 2008
//
// This work is based upon the Robert Bristow-Johnson EQ Cookbook. It also uses
// code by Albert Graef as posted to the Linux Audio Developers mailing list.
// This code is noted via comments. I would like to thank Albert Graef for
// showing me how easy it is to implement filters in FAUST. :-)
//
// This EQ is designed after the behavior of the Neve 1073 channel equalizer,
// using the RBJ cookbook filter formulas. The filters are 12dB/octave, rather
// than the 18dB/octave of the original circuit. Hopefully, it still results in
// a useful EQ.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 675 Mass
// Ave, Cambridge, MA 02139, USA.
////////////////////////////////////////////////////////////////////////////////

declare name        "Channel EQ";
declare version     "1.1";
declare author      "Darren Landrum";
declare copyright   "Darren Landrum";
declare license     "GPLv2";

import("nuclear.lib");

// UI Setup: Low shelf filter
LSCutoff        = hslider("Low-Shelf Cutoff",100,30,300,10);
LSGain          = hslider("Low-Shelf Gain (dB)",0,-20,20,0.1);

// UI Setup: Mid-peak filters
MP1Cutoff       = hslider("Mid 1 Cutoff",1600,300,10000,10);
MP1Gain         = hslider("Mid 1 Gain (dB)",0,-20,20,0.1);
MP2Cutoff       = hslider("Mid 2 Cutoff",1600,300,10000,10);
MP2Gain         = hslider("Mid 2 Gain (dB)",0,-20,20,0.1);

// UI Setup: High-shelf filter
HSCutoff        = hslider("High-Shelf Cutoff",12000,10000,18000,0.1);
HSGain          = hslider("High-Shelf Gain (dB)",0,-20,20,0.1);

// UI Setup: Output level control
Output          = hslider("Output Gain (dB)",0,-30,10,0.1);

// Main process
process =   rbj_high_shelf(HSCutoff,HSGain):
            rbj_mid_peak(MP1Cutoff,MP1Gain):
            rbj_mid_peak(MP2Cutoff,MP2Gain):
            rbj_low_shelf(LSCutoff,LSGain):
            fader(Output);

// END OF PROGRAM
