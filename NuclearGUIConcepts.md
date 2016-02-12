# Introduction #

It has recently been a topic on my mind about how to move away from emulating the look of classic equipment and create UIs for synthesis and effects control that takes full advantage of the computer as a HUI device. I'm using for reference mainly the look and feel of the TC Electronic Fabrik-R Reverb plug-in(1), which serves as my inspiration.

# A Basic Description #

Each module or node, such as an oscillator or filter, is represented by a puck sitting upon a large surface. When a puck, representing a particular module, is first placed on the surface, it is wired to the other pucks as part of the directed graph. After that, the position of the puck on the field determines the value of various parameters controlling the module. Using a low-pass filter as an example, moving the puck up and down might represent resonance level, while left and right would be its cutoff.

For modulation, instead of envelopes and LFOs and the like, a path can be drawn on the surface that a particular puck will follow. It can do this either over time (with acceleration possibly being defined as well), back and forth over time (looping), in response to the output of another puck, or in response to an event, like a message from a real-time controller.

I believe this basic idea can be extended to pretty much any kind of synthesizer, sampler, or effects unit.

# Layers #

Some thinking about how to avoid a cluttered surface has brought me to a new idea: layers. The surface could be composed of multiple layers which can (but doesn't have to) contain independent graph networks. Multilayered sample presets could be made in such a way. This way, large presets can be worked on in pieces that make sense to the user. It makes sense that only one layer should be visible at a time to avoid clutter.

# Advantages #

  * **Clarity of Programming:** Once the user gets used to the UI design, it becomes very easy to see at a glance everything that is happening in a preset. As a result, creating or modifying presets becomes quite easy.
  * **Encourage Experimentation:** Being able to get to so many parameters visualized on the surface might lead to a great deal of experimentation in sound design.
  * **Multi-touch Surface Ready:** If the user is lucky enough to have a multi-touch table, this GUI will be already perfect for it.

# Disadvantages #

  * **Modules With Too Many Parameters:** Some modules have too many parameters to neatly map to a 2-dimensional surface.
    * _Possible Solution:_ When a puck is dragged to the surface for the first time, it can ask the user which parameters to map to each axis. Other parameters can then be set static. This is the only part of this whole idea that I don't like.

# Conclusion #

It has long been my view that emulating the look of old hardware gear, although it is a nice bit of eye candy in many cases, is also somehow insulting to the intelligence of the user. New thinking has to be encouraged to allow for the creation of new UI paradigms for music creation, and I think this idea is a step in that direction.

# References #

(1) [TC Electronic Fabrik-R Reverb](http://www.tcelectronic.com/FabrikR.asp)