# Introduction #

Everything in this article references a paper by Anders Torger and Angelo Farina, called Real-time Partitioned Convolution for Ambiophonics Surround Sound:

http://pcfarina.eng.unipr.it/Public/Papers/164-Mohonk2001.PDF

# The Algorithm In a Nutshell #

  * Split the impulse response file into several (probably a power of two, like 8 or 16) partitions, perform an FFT on each one.
  * Convolve the incoming signal with all of the impulse partitions.
  * Perform the iFFT on each partition, then append the result to the output stream.

The net result is an FFT operation that would normally be of latency M, the length of the impulse response file, will now be of latency N, the length of a partition. The nice part is the splitting and FFT of the IR, and the final summing, are all parallelizable, and thus can be optimized with SSE2 instructions. The article explains the algorithm in depth, including how to use FFTW and BruteFIR to do the heavy lifting.

# FAUST Concerns #

This is a good example of a node that likely cannot be made with FAUST. Therefore, the node will have to be coded directly from a basic skeleton. Hopefully, this won't cause any issues.