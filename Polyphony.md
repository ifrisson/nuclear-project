# Introduction #

**Q:** What is a "voice"?

**A:** All code that is needed by the voice to create a sound.

Imagine a very crude monophonic synth: To make a sound we just need a single oscillator. When you press down a key the single oscillator is set to the corresponding frequency and gets ungated. When you press down another key the single oscillator is set to the frequency corresponding to the new key, and so on. The monophonic synth has only one voice consisting of a single oscillator.

To make our crude synth polyphonic we need several oscillators in a pool, one per voice (if we want 128 voice polyphony we need 128 oscillators). When a key is pressed down the next available oscillator in the pool is set to the corresponding frequency and ungated on. When another key is pressed down the next available oscillator in the pool is used and so on (until all voices are used up - when that happen we need a voice stealing algorithm to decide what to do).

Of course a single oscillator isn't very interesting, most synth voices consist of three general part: source, modifier and sink. The source might be an oscillator or a sample player, the modifier some kind of filter(s) and an amplifier controlling the panning and attenuation and the sink somewhere to output the signal. Also belonging to the voice is any modulators (EGs, LFOs and what-have-you). All those things, used by the voice to make a sound, need to be duplicated for each voice.

**Q:** Doesn't that mean there can only be one voice type per synth?

**A:** Yes, that's correct. But it really doesn't matter.

There are actually no case where you would need more than one defined voice type per synth, just layer the different signal paths and switch between them. You can easily have an arbitrary number of alternative signal paths per voice to switch between.

As an example, let's get back to our crude polyphonic synthesizer: Say we want to have one type of voice based on a sinus oscillator and another based on a sawtooth oscillator. The type of voice is in our case selected by the note on velocity, 0-63 result in a sinus wave and 64-127 in a sawtooth. To achieve it we simply add both oscillators in parallel to our synth and add a routing block downstream that will select one of the inputs depending on the note on velocity and pass the returned value on to the output.