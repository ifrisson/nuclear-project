/* -*- Mode: C++ ; c-basic-offset: 8 -*- */

// Test wrapping an engine as LV2 plugin

#include "../sdk/engine.h"
#include "../sdk/lv2/common/lv2_plugin.h"

class test_engine :
	public nuclear::engine
{
public:
	test_engine()
	{
	}

	~test_engine()
	{
	}

	void init(nuclear::uint32_t srate)
	{
	}

	void activate()
	{
	}

	void deactivate()
	{
	}

	void run(nuclear::nframes_t nframes, nuclear::sample_t** inputs, nuclear::sample_t** outputs)
	{
	}
};

class test_plugin
{
public:
	static nuclear::string_t uri()
	{
		return "http://nuclearproject.org/sdk/test/lv2_plugin/1";
	}

	static nuclear::lv2::port_type_t port_type(nuclear::uint32_t index)
	{
		switch (index)
		{
		case 0:
			return nuclear::lv2::CONTROL;
		case 1:
			return nuclear::lv2::AUDIO_OUT;
		default:
			return nuclear::lv2::NONE;
		}
	}	
};

static nuclear::lv2::plugin<test_plugin, test_engine> _ = nuclear::lv2::plugin<test_plugin, test_engine>();
