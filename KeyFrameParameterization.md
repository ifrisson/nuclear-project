# Introduction #

This page details some brainstorming I (Darren Landrum) had about a way to control and route exposed internal parameters of a synthesis/sampling engine so that clever routing, linking, and processing of control signals could re-create any feature wanted in a synth.

The idea is based on the concept of setting key-frames on one or more exposed parameters, and animating between the two frames. The animation could be linear, or along a defined curve, and it could be controlled by a timer, or by an external control, including another key-frame pair. The idea was inspired by the way key-frame animation is done within 3D rendering software.

Although I will try to explain this as simply as possible, some knowledge of functional math will likely be required. Sorry.

# The Basics From a User Perspective #

At the level of the finished synth/sampler (which I'll refer to as just the "synth" from now on), there would be no envelopes, LFOs, or any other automated control method for any parameter directly available. Instead, the user would set a parameter to the point he or she (I'll say "he" from now on, just to make things easier on me and because the English language doesn't provide a third-party gender-neutral pronoun) wants it at to start with, right-click on the parameter "knob" (or slider, or whatever other widget is provided), then set that as a key frame. Then the user sets the parameter widget to a new value that he wants, right-clicks, and sets that as another key frame.

## The Envelope, Please ##

At this point, the user can now make a few decision about how the parameter will actually behave, given these two key frames. He can set a number of seconds from key-down that it'll take for the parameter to move from the first key frame to the second. He can also choose which curve the parameter will follow over this interval of time, whether that be linear, exponential, or something custom. This is called the _function_ of the key frame pair, and would be implemented as a mathematical function, with x or t as the controlling variable, and y as the output, which will be the value applied to the parameter. At this point, for time-controlled key frame pairs f(t), further key frames can be defined on the same parameter and set sequentially to create even more complex envelopes.

That does seem a long way to go to make some envelopes, even as flexible and powerful as these would be. However, time is not the only thing that might control the movement between two key frames. Movement could also be controlled by a real-time MIDI control, like the mod wheel, for instance. This movement would still happen along any pre-defined curve, which could literally take on just about any shape.

## But It Gets Better ##

The real power happens when the output of one set of key frames is then used to control the movement between another set of key frames, which in turn controls one (or possibly more) parameters. Now, if the output of the first key frame set is just a complex curve like is possible, you'd end up with a very complex (and probably unpredictable) output from the second key frame set. However, the output of the first curve can be dependent on something more predictable, like which key on the (musical) keyboard was pressed, this first function can then change movement along the x axis of the second function, and thus how it controls the parameter(s) it is in charge of. Furthermore, if a function exposes parameters that control the shape of its curve (by way of altering the values of its polynomial coefficients, for example), these can also be controlled by a different key frame pair.

This is where the real power of this idea comes to fruition. By treating all of the data, including control data, flowing through the synth as signals to be processed and created by DSP synthesis methods, allowing the user to create new functions, and allowing the routing of signals and triggers to and from anywhere, including other control functions, it is my thesis that any feature that has ever been asked for or implemented on any software synthesizer or sampler can be implemented _at the user (patch) level_, as long as the required parameters of the core engine (such as filter cutoff, sample loop points, etc.) are exposed.

## More About Triggers ##

I threw this term out there without really explaining it. Basically, each key frame pair, if it's time controlled, needs to know when to start running. By default, for a single key frame pair, or the first of a serial set, the default trigger would be key-down, though that could be changed. For any key frame pair, the trigger can be anything, really. For the final frame pair of a serial set, the trigger might actually be the MIDI note off message, thus defining the last point of the next-to-last key frame as the sustain point.

# Let's Explore Some Possibilities (Examples) #

## LFOs ##

This would require one more feature to be available to time-controlled key frame pairs: the ability to start over once the end point is reached (looping). If we can do that, very complex LFO shapes can be made and routed anywhere.

## Sample Loop Point Modulation ##

This would be implemented by two different key frame pairs, each containing the exact same function (curve), and both controlled by the same source, whether that be time, another function, or a real-time control input.

The starting loop point is hooked up to one key frame pair, and the ending loop point to the other. As the control signal to the key frame pairs changes, the loops points will move in tandem.

Of course, one problem with this is that the loop points would change smoothly and would inevitably end up on non-zero-crossings, which would make a lot of pops. One solution would be if the function of the key frame pairs could know where every zero-crossing in the wave is, information which could be provided by the engine. Another solution would be cross-faded loops.

## Cross-Faded Loops ##

I can envision in my head how cross-faded loops can be made with key frames, but I'm finding it difficult to describe explicitly. I'll point out a few things, though:

  * The amplitude of the wave playback is a parameter controllable by key frames.
  * Nothing says the sample playback object has to provide the looping functionality. As long as the start point of the playback of the loaded sample can be controlled, key frames can create sample playback loops.
  * And now we can overlap two different loops on the same wave. The only catch is the wave playback object would need to be able to output playback from both of two different playback points on the same wave for the period the cross-fade lasts. This is an example of where designing the engine for the key framing system becomes important.

You can probably take it from there. This is actually one of the most complex things one can do with key frames, I think, as it actually starts to border on programming to the engine level, rather than to the patch level.

## Scaled Envelopes ##

This would be a matter of picking how to control the scaling, say the MIDI note number, and using that as the control input for a key frame function, which then outputs a signal that controls how fast another key frame pair moves along its function.

## Audio Rate Key Frames ##

And we haven't even considered cases where key frame functions are running at audio rates. I won't really expand on this, but suffice it to say, the possibilities are quite interesting.

# Conclusion #

I think that about sums it all up. By allowing the user to:

  * Define key frames of a parameter
  * Define a function f(x) or f(t) to determine what path the parameter takes between those key frames
  * Define a control to determine where along the function the parameter value is at any particular moment
  * Allow the control of key frame functions by other key frames

A myriad of synthesizer and sampler programming possibilities begins to open up for the user. There are some caveats, though:

  * The engine needs to be designed with key frame-based sound design in mind
  * Some features start to require fairly convoluted combinations of key frame pairs and functions

Now, since I first developed these ideas, the sampler project I had created these ideas for has moved on into the graph-based synth/sampler creation design now called Nuclear. I am confident that these caveats can be solved, and that a key frame-based system of sound design can be implemented at both the instrument-building and patch-building levels. The important points are to:

  * Treat all information flowing through the graph as signals which can be processed by DSP.
  * Allow the easy creation of new nodes for the creation/modification of either audio or control signals.
  * Allow nodes access to external signal data (from MIDI, for example) so that they can be programmed to trigger upon any event.
  * Allow any signal to be routed to and from anywhere.

Thank you for reading!