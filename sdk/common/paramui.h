#ifndef NUCLEAR_PARAMUI_H
#define NUCLEAR_PARAMUI_H

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

#include "../UI.h"
#include <string>
#include <stack>
#include <map>

namespace nuclear
{

class paramui :
	public UI
{
	struct param 
	{
		param(float* z, float a, float b) : 
			zone(z), 
			min(a), 
			max(b) 
		{}
		
		float* zone; 
		float min; 
		float max;
	};

public:
	paramui() :
		UI()
	{
	}

	~paramui()
	{
	}

	virtual void addButton(std::string label, float* zone) 
	{
		add_option(label, zone, 0.0f, 1.0f);
	}

	virtual void addToggleButton(std::string label, float* zone)
	{
		add_option(label, zone, 0.0f, 1.0f);
	}

	virtual void addCheckButton(std::string label, float* zone)
	{
		add_option(label, zone, 0.0f, 1.0f);
	}

	virtual void addVerticalSlider(std::string label, float* zone, float init, float min, float max, float step)
	{
		add_option(label, zone, min, max);
	}

	virtual void addHorizontalSlider(std::string label, float* zone, float init, float min, float max, float step)
	{
		add_option(label, zone, min, max);
	}

	virtual void addNumEntry(std::string label, float* zone, float init, float min, float max, float step)
	{
		add_option(label, zone, min, max);
	}
	
	virtual void addNumDisplay(std::string label, float* zone, int precision) {}
	virtual void addTextDisplay(std::string label, float* zone, char* names[], float min, float max) {}
	virtual void addHorizontalBargraph(std::string label, float* zone, float min, float max) {}
	virtual void addVerticalBargraph(std::string label, float* zone, float min, float max) {}
	
	virtual void openFrameBox(std::string label) 
	{ 
		add_namespace(label); 
	}

	virtual void openTabBox(std::string label) 
	{ 
		add_namespace(label); 
	}

	virtual void openHorizontalBox(std::string label) 
	{ 
		add_namespace(label); 
	}

	virtual void openVerticalBox(std::string label) 
	{ 
		add_namespace(label); 
	}

	virtual void closeBox() 
	{ 
		_prefix.pop(); 
	}

	void set_option(std::string label, float value)
	{
		std::map<std::string, param>::iterator p = _params.find(label);
		if (p == _params.end())
			throw "Unrecognized option";

		if (value >= p->second.min && value <= p->second.max)
			*(p->second.zone) = value;
	}

private:
	void add_option(std::string label, float* zone, float min, float max)
	{
		_params.insert(std::make_pair(_prefix.top() + label, param(zone, min, max)));
	}

	void add_namespace(std::string label)
	{
		if (_prefix.empty())
			_prefix.push(label + "/");
		else
			_prefix.push(_prefix.top() + label + "/");
	}
	
	std::stack<std::string> _prefix;
	std::map<std::string, param> _params;
};

} // !namespace nuclear

#endif // !NUCLEAR_PARAMUI_H
