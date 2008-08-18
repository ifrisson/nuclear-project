#ifndef NUCLEAR_UI_H
#define NUCLEAR_UI_H

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

namespace nuclear
{
	
	namespace faust
	{
		
		/// Abstract base class for FAUST UI builder
		class UI
		{
		public:
			
			UI() {}
			virtual ~UI() {}
			
			// -- active widgets
			
			virtual void addButton(std::string label, float* zone) = 0;
			virtual void addToggleButton(std::string label, float* zone) = 0;
			virtual void addCheckButton(std::string label, float* zone) = 0;
			virtual void addVerticalSlider(std::string label, float* zone, float init, float min, float max, float step) = 0;
			virtual void addHorizontalSlider(std::string label, float* zone, float init, float min, float max, float step) = 0;
			virtual void addNumEntry(std::string label, float* zone, float init, float min, float max, float step) = 0;
			
			// -- passive widgets
			
			virtual void addNumDisplay(std::string label, float* zone, int precision) = 0;
			virtual void addTextDisplay(std::string label, float* zone, char* names[], float min, float max) = 0;
			virtual void addHorizontalBargraph(std::string label, float* zone, float min, float max) = 0;
			virtual void addVerticalBargraph(std::string label, float* zone, float min, float max) = 0;
			
			// -- frames and labels
			
			virtual void openFrameBox(std::string label) = 0;
			virtual void openTabBox(std::string label) = 0;
			virtual void openHorizontalBox(std::string label) = 0;
			virtual void openVerticalBox(std::string label) = 0;
			virtual void closeBox() = 0;
		};
		
	} // !namespace faust
	
} // !namespace nuclear

#endif // !NUCLEAR_UI_H
