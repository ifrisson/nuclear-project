# Introduction #

It was and continues to be a seminal instrument of the 70s and 80s. The CS series, including the CS80, was Yamaha's second shot at a synthesizer for the mass market after years of making pianos and organs. Their first go, the GX-1, was a USD$80,000 multi-keyboard monster, which you can see in action, helmed by Keith Emerson, in this [Youtube video](http://www.youtube.com/watch?v=0OLWgrr671g).

Eventually, the technology of the GX-1 trickled down into the CS80, which was 8-note polyphonic, fully analog, and cost about USD$8,000, brand new. Still pricey, but within the reach of many electronic artists of the day. Unfortunately, the release of the Sequential Circuits Prophet-5, at a mere $2,500, means the CS80 didn't do too well in the end, but it still found itself in the hands of Toto, Vangelis (who swore by it), and the [BBC Radiophonics Workshop](http://www.youtube.com/watch?v=-OWMGAiaNeQ).

## A Note About This Article ##

I just want to point out that as of right now, this article is little more than brainstorming. It is my hope, though, that it will eventually evolve into an effective design document for a Nuclear-powered emulation of the sound of the famous CS80.

# The Inspiration #

Other than the original CS80, there is a software synth that made me believe this was a possible project. It is _not_ the Arturia CS80V. It is, instead, an inexpensive VSTi plug-in made by one man: the [Olga](http://www.stillwellaudio.com/?page_id=37), by Schwa. Just take a listen to the mp3 samples and you'll hear what I mean.

# Research #

[Antti](http://antti.smartelectronix.com/), of Smartelectronix, has a [paper](http://dafx04.na.infn.it/WebProc/Proc/P_061.pdf) available on modeling the non-linear character of the Moog filter. While this is not quite what we're after for the CS80, we can learn something from his approach.

# Considerations #

All electronic circuits have non-linear behaviors, which result in forms of distortion. One of the goals of the electronic designer is to maximize the linear range of his design, and make sure that the device operates within those linear ranges as much as possible. Another goal is to make sure that when a device does get driven beyond its linear range, it does so "gracefully" (ie, with as little intermodulation distortion as possible). There are exceptions to these goals, of course, guitar amps being a famous example. Many tube mic preamps are also designed to be driven a little into their non-linear ranges for the purpose of adding "warmth" to a signal.

Nevertheless, a lot of electronic devices, such as the CS80, have non-linear effects that are very pleasing to the ear. Whether the designers intended this or not is moot. Sound designers quickly learned how to take advantage of these effects to create sounds with a lot of character.

The point here is that it's not just the _filters_ that will exhibit these distortions, but _all_ of the circuits that make up a synthesizer. A good approach at this point, then, would be to look at the signal path of the CS80, break it up into its different block sections, then decide how to handle the design of each section to take into account both its function and its distortions.

# Architecture #

Rather than stealing a bunch of text, I'll just link over to the very nice [description](http://www.synthmuseum.com/yamaha/yamcs8001.html) on the Synth Museum web site. Here are the highlights:

  * 1 VCO, simultaneously outputting square, sawtooth, and sine, and LFO for PWM.
  * 1 low-pass VCF, followed by 1 high-pass VCF, each 12dB/oct with resonance, each with a simple ASR-type envelope.
  * VCA with ADSR envelope.
  * LFOs for sweeps, vibrato, tremolo. (These were actually "global", but we'll ignore that fact for the purposes of our simulation.)
  * The catch is that there were actually **two** of these "voices" per voice, independent of each other. Thus, each voice was essentially running two patches at once.

Globally, the CS80 had:

  * Ring modulator, with its own modulation oscillator, and some other quite unique features highlighted in the afore-linked article.
  * An LFO section that affected both running patches equally. We'll ignore this for the purpose of our simulation and implement LFOs per half-voice like most users would expect. (It's like _deja vu_ all over again...)

The following is some high-level brainstorming about how to go about each of these functions.

## Oscillator ##

I've become a big fan of the idea of using Bezier curve functions to create anti-aliased oscillators. Not only can they make a wide variety of wave shapes, those shapes can be changed on the fly in creative ways. This would help in implementing PWM, for example.

## Low-Pass Filter ##

A 12dB/octave resonant low-pass filter is nothing special, really. Of course, we're also wanting to think non-linearly.

## High-Pass Filter ##

The same considerations as for the low-pass apply here, I think.

## VCA ##

You can design the VCA to where all it does is multiply the input signal by the output of the envelope generator, then send the output on its way. Or, you can design subtle distortions into a non-linear range that sound designers can push against. Exactly how to do that is subject to a lot of debate and trade secrets. I intend to figure it out.

## Ring Modulator ##

From the [Synth Museum](http://www.synthmuseum.com/yamaha/yamcs8001.html) web site on the CS80:

> The Ring Modulation section is quite elaborate, with sliders for modulation, speed, depth, attack time, and decay time. Modulation controls how much of the ring-mod signal is present in the sound compared to the original signal. Speed determines the basic frequency of the modulation signal. The depth, attack time, and decay sliders make up a simple AR envelope generator for controlling the frequency of the modulation signal ("speed"). The depth slider sets the maximum frequency reached, and the attack and decay sliders set how long it takes the modulation to speed up to its maximum frequency and how long it takes to slow down again. No other synth manufacturer has implemented a ring mod section in quite the same way. This is a part of the CS series' signature sound.

I suppose implementing all of that would be a good start. Of course, we'd also want to think about how a ring modulator might distort.