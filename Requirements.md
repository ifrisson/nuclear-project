# Background #

The Nuclear project sprung from a discussion over at the [LinuxSampler forum](https://bb.linuxsampler.org/viewtopic.php?f=5&t=101).

Nuclear is neither a synthesizer nor a sampler. It's beyond that. Instead we let you define new synthesizers, samplers and effects with ease. The vision is to give virtual instrument makers and plugin developers a powerful tool to create instruments without being limited by a single synthesizer/sample player or bogged down in implementation details common to all virtual instruments.

Originally the idea was to create a single integrated application that both engine creators and end-users would use. An engine maker would use the built in GUI editors to create DSP blocks and connect them together in a processing graph. The DSP and processor graph declarations would be stored in platform independent formats. When a user loaded the engine all the DSP declarations would be compiled natively as modules and the processing graph would be built dynamically from the engine declaration file. This would require the end user having a compiler and build environment installed and properly configured to be used by the application.

The revised idea was to create a framework to generate code from DSP block and engine [declarations](http://en.wikipedia.org/wiki/Declarative_programming) that would be compiled and linked as a native library. The intended audience was developers who wished to create sample players, synthesizers and effects using a declarative method. To the end-user all hosts and engines would appear and be used just like regular applications or plugins.

Our current direction is to develop an SDK to make the job of writing new plugins and applications a breeze. Leveraging on free software technologies such as LV2, JACK and Faust.

# Features #

  * Being able to create:
    * software synthesizers;
    * sample players;
    * effects and filters.
  * Compile as:
    * [JACK](http://jackaudio.org) application;
    * [LV2](http://lv2plug.in) plugin.
  * Use [FAUST](http://faust.grame.fr/) for DSP development.
  * Creating engines by interconnecting DSP blocks as nodes in a processing graph:
    * processing graphs are declared in a XML schema;
    * [oversampling](Oversampling.md) of local portions of the graph;
    * grouping of nodes for reuse to create custom DSP routines from basic building blocks.
  * Scripting based on [OpenSound Control](http://opensoundcontrol.org). Inspired by the [OSC 'SYN' namespace](http://stud3.tuwien.ac.at/~e0725639/OSC-SYN.txt) proposal.
  * Develop good looking custom GUIs for instruments and effects.
  * Use [CMake](http://www.cmake.org/Wiki/CMake) for build automation of the engines.