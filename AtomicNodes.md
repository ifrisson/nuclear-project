# Introduction #

This page describes the type and implementation of the basic nodes that can be combined to create a wide variety of DSP blocks.

Individual nodes can be combined into groups of nodes, which can then be treated as a cohesive unit by the graph. These are called _node groups_, or simply _groups_. Although any node can be made part of a group, regardless of its complexity, there will exist as a core part of Nuclear a set of basic DSP nodes, which are called _atomic nodes_, that will allow for the creation of a wide variety of DSP algorithms when grouped together.

# The Atomic Nodes #

In no particular order:

### The Arithmetic Nodes ###

These should be pretty self-explanatory. Two signals enter, one signal leaves. Input signals can be constants, from user controls, or audio. There will be four separate nodes, one each for addition, subtraction, multiplication, and division.

### Constant ###

This outputs a signal of a constant value. This is not likely to be a commonly used node, but if you ever need it, it'll be nice that it's there.

### Variable Delay Line ###

This is a delay line whose length can be modulated in real time, and can be tapped as well. You might be asking, though, where the other simple types of delay lines are. Well, what do you call a variable delay line whose length is _not_ modulated? A fixed delay line! This delay line can be of length 1 all the way up to any theoretical N in size, depending on hardware.

How to make:

  * N-Sample Delay Line: Set the length of the line and don't modulate it.
  * Multi-tapped Line: This requires the use of the delay tap node, described below.
  * Comb filter: Branch off the output to be sent through the delay line, set the desired delay, send the output of the delay to a multiplier to reduce its amplitude, then add back into the main signal at the desired point.

This would be one of the essential atomic elements of DSP, allowing the creation of filters (FIR, IIR, and comb), flangers, phasers, delays (the guitar effect kind), and reverbs (including the fancy shimmering modulation effects).

### Delay Tap ###

This allows a delay line to be tapped at any point N along its length, returning 0 if the tap position doesn't exist. I'm still trying to figure out how this node will work within the logic of signals and graph theory.

### Single Sample Delay Line ###

This is for applications that don't need anything more complicated, such as simpler FIR filters. This simple single-sample delay line will offer higher CPU efficiency than using the more complex variable delay line for such a short time.

### Integrator ###

This keeps a running summation of an input signal available as an output to other nodes:

```
y[n] = x[n] + y[n-1]
```

Notice that this uses the previous output sample as an input to the function. That makes it easy to keep the integration going.

### Differentiator ###

This outputs the slope of the current point of the input signal, based upon running subtraction of the previous sample from the current sample:

```
y[n] = x[n] - x[n-1]
```

That output is then made available as a signal. Another possibly more accurate way to implement this would be:

```
y[n] = (x[n+1] - x[n-1]) / 2
```

This would create a latency of one sample. This might cause an issue on graphs with parallel paths. I'll have to work this out and elaborate on it.

### Time to Samples ###

So, what do you do when you need to convert a time in milliseconds to a number of samples in a system with a user-defined sample rate? You break out the handy-dandy time-to-samples conversion node! This will have one input, one output, and do exactly what it says it does.

### Samples to Time ###

And here's one to do the exact opposite, when you need it. Input is in number of samples, output is time in milliseconds. I don't know yet if this will be a useful node, but for now, it won't hurt, I suppose.

# An Incomplete List #

Although these will allow for the creation of many different DSP algorithms, for now, treat this as an incomplete list of the basic node types. Also, keep in mind that new nodes can be written very easily, and that nodes of more complex functionality can be included in node groups.