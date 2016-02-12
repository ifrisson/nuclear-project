# Introduction #

To handle oversampling we need to have special nodes that take advantage of certain properties of a directed graph.

## Downsampling ##

Imagine a node for downsampling, which implements a low-pass FIR filter and decimator, as is usual. When the graph is walked and this node encountered, this node will then ask for the input it needs in order to pass along its output to the following node. The difference, however, is that it must ask for N number of inputs, where N is the number of times this region of the graph is being oversampled, before it can send its one output along.

## Upsampling ##

The upsampling node works the same way, but in reverse. When it asks for and receives an input, it stores the input and then creates N-1 zeros, which are then run through a low-pass polyphase FIR interpolation filter, again as is usual for upsampling operations. Now, when it is asked for its output, it will output all of the samples in its buffer, and not have to ask for a new input for N number of outputs. Everything in between the upsampling node and downsampling node will now be oversampled, and none of the nodes in between will need to be specially designed for the task. They can behave as they always would.