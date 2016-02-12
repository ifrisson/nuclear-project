# Introduction #

The Smart Matrix seeks to solve, in one object, several of the issues concerning Nuclear's DSP engine, namely, that of routing signals to/from the Faust nodes, executing those nodes in the correct order, and automatically handling multirate processing (oversampling).

# The Basic Idea #

Instead of using a digraph system where nodes are connected by telling each node where to receive signals by calling functions in the sending nodes, the Smart Matrix (SM) instead chooses to keep the nodes completely "dumb", being told (each time they are activated to process a block of data) where input data is, where to place the finished block, and how big the block is. It is then up to the SM to provide the data to each node, and to shuffle the result to the next node(s), and to do this in the correct order.

The SM can also automatically handle upsampling and downsampling by comparing how many input samples are being processed vs. how many output samples are being asked for by the programmer. The SM can then automatically "install" the appropriate interpolation or decimation filter at the correct point in the signal chain.

# The Advantages #

The code for node objects can be kept relatively simple. All they need is a run function which takes as arguments a pointer to the input samples, a pointer to place the output at, and the number of samples to be processed. The SM takes care of the rest. This should make it easy to make an architecture file for Faust that will allow its compiled output to drop right into Nuclear.

# The Disadvantages #

Coding the SM class will be somewhat complicated, especially if it is to automatically handle multirate transparently.

# Polyphony for Synths #

Polyphony handling will be a separate object, which will instantiate a number of SM objects, each of which will be a voice. Most synths have a voice section and a global section. The global section will simply be its own SM object for the audio to run through after the voices are summed.

# Conclusion #

By using dumb nodes with a smarter matrix-based object, an extremely versatile and easy-to-use DSP engine can be created for any kind of synth and effect under the sun. For now, this document is sparse and describes only the high-level concepts. In the future, it will be modified as the SM idea is fleshed out.