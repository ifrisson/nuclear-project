# Digital Filters #

## What is a Filter? ##

> "Any medium through which the music signal passes, whatever its form, can be regarded as a filter. However, we do not usually think of something as a filter unless it can modify the sound in some way. For example, speaker wire is not considered a filter, but the speaker is (unfortunately). The different vowel sounds in speech are produced primarily by changing the shape of the mouth cavity, which changes the resonances and hence the filtering characteristics of the vocal tract. The tone control circuit in an ordinary car radio is a filter, as are the bass, midrange, and treble boosts in a stereo preamplifier. Graphic equalizers, reverberators, echo devices, phase shifters, and speaker crossover networks are further examples of useful filters in audio. There are also examples of undesirable filtering, such as the uneven reinforcement of certain frequencies in a room with 'bad acoustics.' A well-known signal processing wizard is said to have remarked, 'When you think about it, everything is a filter.'" -- [Introduction to Digital Filters with Audio Applications](http://ccrma.stanford.edu/~jos/filters/) by Julius O. Smith III

> "It is common in DSP to say that a filter's input and output signals are in the time domain. This is because signals are usually created by sampling at regular intervals of time. But this is not the only way sampling can take place. The second most common way of sampling is at equal intervals in space. For example, imagine taking simultaneous readings from an array of strain sensors mounted at one centimeter increments along the length of an aircraft wing. Many other domains are possible; however, time and space are by far the most common. When you see the term time domain in DSP, remember that it may actually refer to samples taken over time, or it may be a general reference to any domain that the samples are taken in." -- [DSP Guide Chapter 14: Introduction to Digital Filters](http://www.dspguide.com/ch14.htm)

## Digital Filter ##

A digital filter is a system that performs mathematical operations on a sampling, discrete-time signal to reduce or enhance certain aspects of that signal. A digital filter is characterized by its transfer function. Mathematical analysis of the transfer function can describe how it will respond to any input. As such, designing a filter begins with finding an appropriate transfer function for a particular problem.

After a filter is designed, it is must be realized by developing a signal flow diagram that describes how the filter can be implemented. A given filter can be realized in many ways. The most straightforward approach is direct computation of the difference equation, which is practical for small filters, but inefficient and impractical for more complex designs.

In discrete-time systems, the digital filter is often implemented by converting the transfer function to a linear constant-coefficient difference equation (LCCD) via the Z-transform.

> "Every linear filter has an impulse response, a step response and a frequency response. Each of these responses contains complete information about the filter, but in a different form. If one of the three is specified, the other two are fixed and can be directly calculated. All three of these representations are important, because they describe how the filter will react under different circumstances." -- [DSP Guide Chapter 14: Introduction to Digital Filters](http://www.dspguide.com/ch14.htm) by Steve Smith

## Types of Digital Filters ##

Many digital filters are based on the Fast Fourier transform, a mathematical algorithm that quickly extracts the frequency spectrum of a signal, allowing the spectrum to be manipulated (such as to create band-pass filters) before converting the modified spectrum back into a time-series signal.

Another form of a digital filter is that of a state-space model. A well used state-space filter is the Kalman filter

## Finite Impulse Response ##

FIR filters have fixed-duration impulse responses. The impulse response, the filter's response to a Kronecker delta input, is 'finite' because it settles to zero in a finite number of sample intervals. The impulse response of an Nth order FIR filter lasts for N+1 samples, and then dies to zero.

An FIR filter has a number of useful properties which sometimes make it preferable to an infinite impulse response filter. FIR filters:

  * Are inherently stable. This is due to the fact that all the poles are located at the origin and thus are located within the unit circle.
  * Require no feedback. This means that any rounding errors are not compounded by summed iterations. The same relative error occurs in each calculation.
  * They can be designed to be linear phase, which means the phase change is proportional to the frequency. This is usually desired for phase-sensitive applications, for example crossover filters, and mastering, where transparent filtering is adequate.

To design a filter means to select the coefficients such that the system has specific characteristics. The required characteristics are stated in filter specifications. Most of the time filter specifications refer to the frequency response of the filter. There are different methods to find the coefficients from frequency specifications:

  * Window design method
  * Frequency Sampling method
  * Weighted least squares design
  * Minimax design
  * Equiripple design.

Some of the time, the filter specifications refer to the time-domain shape of the input signal the filter is expected to "recognize". The optimum matched filter is to sample that shape and use those samples directly as the coefficients of the filter -- giving the filter an impulse response that is the time-reverse of the expected input signal.

## Infinite Impulse Response ##

IIR filters have an impulse response function which is non-zero over an infinite length of time. IIR filters have internal feedback and may continue to respond indefinitely.

In digital IIR filters, the output feedback is immediately apparent in the equations defining the output. Note that unlike with FIR filters, in designing IIR filters it is necessary to carefully consider "time zero" case in which the outputs of the filter have not yet been clearly defined.

Design of digital IIR filters is heavily dependent on that of their analog counterparts because there are plenty of resources, works and straightforward design methods concerning analog feedback filter design while there are hardly any for digital IIR filters. As a result, mostly, if a digital IIR filter is going to be implemented, first, an analog filter (e.g. Chebyshev filter, Butterworth filter, Elliptic filter) is designed and then it is converted to digital by applying discretization techniques such as Bilinear transform or Impulse invariance.

Example IIR filters include the Chebyshev filter, Butterworth filter, and the Bessel filter.

(Source: Wikipedia.)

## See also ##

  * [Digital Signal Processing for HCI](http://soundlab.cs.princeton.edu/learning/tutorials/DSP/DSP.html)
  * [Introduction to Digital Filters with Audio Applications](http://ccrma.stanford.edu/~jos/filters/)
  * [DSP Guide Chapter 14: Introduction to Digital Filters](http://www.dspguide.com/ch14.htm)