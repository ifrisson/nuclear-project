#ifndef PRESETMANAGER_H_
#define PRESETMANAGER_H_

// preset_manager.h
// Copyright (c) 2008 by Darren Landrum
//
// Contact: dmlandrum@gmail.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "types.h"
#include "error_strings.h"
#include <string>
#include <map>
#include <list>
#include <fstream>
#include <boost/lexical_cast.hpp>

namespace nuclear
{

class PresetManager
{
private:
	map<string_t, string_t> preset;
	list<string_t> tags;
	
public:
	PresetManager()
	{
	}
	
	void readPreset(string_t filename)
	{
		string_t line;
		fstream file;
		string_t key;
		string_t value;
		int16_t delim;
		
		// empty the current preset and tag list
		preset.clear();
		tags.clear();
		
		file.open(filename.c_str(), fstream::in);
		
		// parse the file
		while(!file.eof())
		{
			file.getline(line, 1024);
			// split the line into key:value pairs, omitting the delimiter
			delim = line.find(':');
			key = line.substr(0, delim-1);
			value = line.substr(delim+1, line.length()-(delim+1));
			preset[key] = value;
		}
		
		delim = 0; // re-initialize, to be on the safe side
		// extract tags into separate list
		delim = preset["tags"].find(',');
		while(delim != string::npos)
		{
			tags.push_front(preset["tags"].substr(0, delim-1));
			preset["tags"] = preset["tags"].substr(delim+1, line.length()-(delim+1));
			delim = preset["tags"].find(',');
		}
		preset.erase("tags");
	}
	
	void writePreset()
	{
		
	}
	
	template <class T>
	void setParam(string_t param_name, T param)
	{
		preset[param_name] = boost::lexical_cast<string_t>(param);
	}
	
	template <class T>
	T getParam(string_t param_name)
	{
		return boost::lexical_cast<T>(preset[param_name]);
	}
	
	void removeParam(string_t param_name)
	{
		if(param_name != "preset" && param_name != "preset version" &&
		   param_name != "program" && param_name != "program version")
		{
			preset.erase(param_name);
		}
	}
	
	void addTag(string_t tag)
	{
		if(tag.find(',') == string::npos)
		{
			// no commas allowed
			tags.push_front(tag);
			tags.sort();
		}
	}
	
	void removeTag(string_t tag)
	{
		tags.remove(tag);
	}
	
	string_t* getTagList()
	{
		
	}
};

} // !namespace nuclear

#endif // !PRESETMANAGER_H_
