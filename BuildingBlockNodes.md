# Introduction #

This page describes the type and implementation of the basic nodes that can be combined to create a wide variety of DSP blocks.

As the [Grouping](Grouping.md) page explains, individual nodes can be combined into groups of nodes, which can then be treated as a cohesive unit by the graph. These are called _node groups_, or simply _groups_. Although any node can be made part of a group, regardless of its complexity, there will exist as a core part of Nuclear a set of basic DSP nodes, which are called _building block nodes_, that will allow for the creation of a wide variety of DSP algorithms when grouped together.

# The Building Block Nodes #

In no particular order:

### The Arithmetic Nodes ###

These should be pretty self-explanatory. Two signals enter, one signal leaves. Input signals can be constants, from user controls, or audio. There will be four separate nodes, one each for addition, subtraction, multiplication, and division.

### Single Sample Delay Line ###

This node simple holds onto the value input into it for one sample cycle, before it outputs it and then stores the next input. It has one input and one output. This node will be used mainly for the creating of FIR and IIR filters, although it might find other uses as well.

### Multi-Tapped Delay Line ###

This is a delay line of a length which is set at its time of initialization, in samples (and perhaps in milliseconds as well). It can then be tapped at various points throughout its length to get the output of that particular point in the line. Also useful for making FIR filters. It can also serve as the basis for the basic delay effect with long enough delay times.

### Variable Delay Line ###

This is a delay line whose length can be changed at the sample rate. It cannot be multi-tapped, however. This would be useful for creating chorus and flanging effects.

### Integrator ###

An integrator keeps a running summation of an input signal available as an output to other nodes.

# An Incomplete List #

Although these will allow for the creation of many different DSP algorithms, for now, treat this as an incomplete list of the basic node types. Also, keep in mind that new nodes can be written very easily, and that nodes of more complex functionality can be included in node groups.