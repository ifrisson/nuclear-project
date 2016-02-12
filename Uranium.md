# Introduction #

There are barely less guitar amp sims available on the Internet and commercially than can be counted with a 16-bit integer. Why would I want to make another one? Well, first off, none of them are open-source. Second, and most importantly, all of them focus on and strive toward perfect emulation of real guitar amp and cabinets. While that's a worthwhile goal, I have in mind instead a slightly different idea: pleasing sound, EQ, effects, and distortion that don't quite sound like anything else out there, on purpose. This is what Uranium will be about.

Uranium will also serve as Nuclear's test case for non-linear processing of audio. No doubt the application will start small, but grow over time.

# Architecture #

There will be four basic forms of processing available to route together in any number of ways: distortions, equalizers, effects, and cabinet simulations. Technically, _distortion_ would be called an effect, but as guitarists generally treat the distortion of their signals separately from applying other effects, we will as well.

## Distortions ##

Digitally, distortion is created by way of a _waveshaper_. A waveshaper is a memoryless algorithm which outputs a changed signal based on the amplitude of the input. In short, the input amplitude determines what the output will be, based upon a curve determined by a functional equation, such as [hyperbolic tangent](http://mathworld.wolfram.com/HyperbolicTangent.html) or [Chebyshev polynomials](http://mathworld.wolfram.com/ChebyshevPolynomialoftheFirstKind.html). Hyperbolic tangent waveshapers are decent at getting tube-like tones, though they aren't exactly there. Chebyshev polynomial waveshapers allow for very precise definitions of which harmonics in a signal will be distorted, at the expense of requiring oversampling.

The waveshaping curve can also be determined by analysis of a real tube or solid-state amplification and/or distortion stage circuit. The final piecewise-defined (meaning different functions get switched in and out for different domain ranges) waveshaper can then be made based upon least-squares curve fitting. This will also require oversampling to be available. More detail on this can be found in this [paper](http://ccrma.stanford.edu/~dtyeh/papers/yeh07_dafx_distortion.pdf) by David Yeh.

As the application matures, I hope that all three methods will be represented and available to end-users.

### Possibility: Bezier-based Waveshaping? ###

The heading says it all, really. I wonder if it would be possible to use Bezier functions to create waveshapers of just about any shape. It would certainly create a great deal of flexibility. It would also allow for modulating the actual curve of the waveshaper in real time, which could be useful.

This will be worth looking into, I think.

### Distortion Duty Cycle ###

Here's some information about modulating the duty cycle of distortion, as discussed in the Geofex FAQ entry on [distortion](http://www.geofex.com/effxfaq/distn101.htm) (special thanks to R. G. Keen of geofex.com):

> Duty Cycle: Based on an article in Electrical Engineering Times, continuously changes in the relative amounts of clipping between the positive and negative halves of the signal may be an important factor in distortion sound. This article (by Brian Murphy, interviewing John Murphy, who used to be chief engineer at Carvin; EET, October 3, 1994) outlines Murphy's ideas on the degree to which he can simulate tube distortion in solid state designs. He holds that if you filter properly, soft clip, and arrange for the duty cycle of the clipped signal to vary with varying signal levels, you get something very close to tube distortion. This is the basis of the "tube emulation" circuitry in Carvin's SX series of amps.

In fact, here's a link to the [original article](http://www.trueaudio.com/at_eetjlm.htm).

### Intermodulation Distortion ###

I'd like to talk briefly about the kind of distortion we do **not** want to make. Intermodulation distortion is the creation of frequencies in the signal that are **not** harmonics of any part of the original signal. A large part of the reason for the difference between solid-state distortion and tube distortion has to do with intermodulation artifacts.

Tubes tend to avoid a lot of intermodulation by approaching their saturation point slowly, ie, the tops and the bottoms of the wave don't get chopped off so much as they get rounded off. Solid-state tends to be less forgiving, though there are ways to design solid-state distortion circuits that are more pleasing to the ear.

You can read more at the already-linked [Geofex FAQ](http://www.geofex.com/effxfaq/distn101.htm).

## Equalizers ##

Two forms of equalizer will exist in Uranium. One will be in emulation of the basic one-pole passive equalizers present in the majority of guitar amps. The other kind will have steeper cutoffs and be designed more for frequency splitting and cross-over duties for multiband processing. More on that later.

## Effects ##

Obviously, there are many effects that could be created as part of the total package. There are literally hundreds of different designs for stomp-boxes, flangers, delays, and the like. Here are some classes of effects that would be nice to have:

  * Flangers and Phasers
  * Delays - simulating a tape-based delay, such as the Roland Space Echo, would be a fascinating challenge.
  * Filters and Wahs
  * Tremelo

What kind of characters one would want these to have would determine the best approach to take for each one. Clearly, Uranium as a whole will be quite modular in nature.

## Speaker and Cabinet Emulation ##

A cabinet is basically a two-pole (closed cabinet) or four-pole (open cabinet) low-pass filter with a very complicated response that is hard to simulate with standard IIR filters. Furthermore, the speakers themselves add their own filtering plus a host of non-linear distortion effects, including cone breakup.

The easy way to simulate a cabinet is to take sine wave sweeps of real guitar cabinets, using good mics in a good room, and using those to create _impulse response_ files for a convolution processor. This is effective at emulating the _linear_ properties of many good guitar cabinets. However, convolution is unable to simulate any of the non-linear characteristics of the speaker drivers. Nevertheless, this can get 90% of the way there, and in fact this is how many commercial guitar amp sims work.

Simulating the non-linear character of speakers might not be possible with normal waveshapers, as the distortions created are physical in origin, rather than electronic. I've done some basic research into this problem, and will continue to research and experiment as Uranium grows into a useful application.

# Putting It All Together #

## Multiband Distortion ##

In the late 80s, Craig Anderton put together a DIY kit called the Quadrafuzz, which used LEDs as distortion elements, but just as importantly, split the guitar signal into four separate bands before distorting each one separately. The Quadrafuzz is still available as a [Paia kit](http://www.paia.com/proddetail.asp?prod=6720K). Craig himself also explains the circuit [here](http://www.paia.com/ProdArticles/quadrafz-design.htm). The entire idea was to greatly reduce intermodulation distortion while allowing a wide variety of tones to be created.

Needless to say, Uranium would make creating such multiband distortion a breeze. The waveshaping node itself will be modeled on the measured response of an actual LED clipper, or any other similar diode clipper.

(More will be added to this section.)

# The Needed Nodes #

Uranium will depend heavily on the development of waveshaping nodes. Not only the basics like tanh(x) and Chebyshev polynomials, but waveshapers that follow custom curves, and piecewise-defined curves based on real equipment, will be necessary.

## Real Guitar Amp Stages ##

These will be piecewise-defined waveshapers based on the measured curves of single stages of actual guitar amps. The math for these will get complicated really quick, and will require a lot of research to do well. These will be implemented as a custom-coded node for each curve.

## Polynomial and Hyperbolic Functions ##

It will be likely that a node will be made for each added order of Chebyshev polynomial, as you need more oversampling the higher the order used. The coefficient of each available order can be parameterized, to control the final response of the waveshaper.

The hyperbolic functions are a little simpler, and will likely be implement as polynomial approximations, one per function. Depending on the order of the approximation, these will also require oversampling to avoid aliasing.

## Bezier Waveshaper ##

Should it prove possible, these could be really valuable.

## 4-Pole Filters ##

Nodes will need to be created for 4-pole low-pass, high-pass, and band-pass filters. This is the kind of thing FAUST excels at.

## 1-Pole Filters ##

These will be derived from the basic passive EQ types of circuits most guitar amps have. One each will be needed for low-pass, band-pass, and high-pass. Again, FAUST excels at this sort of thing.

## Convolution ##

This will require a collection of nodes. Further research will need to be done. As more details are known, they will be added here.

# Conclusions #

Nuclear should make possible the creation of a high-quality guitar processor that can create new and pleasing sounds that are somewhat familiar, but not quite so. Theoretically, it should also be possible to fairly closely simulate a number of amps, as long as those amps can be physically accessed and analyzed in the way needed.