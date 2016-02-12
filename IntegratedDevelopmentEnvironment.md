# Introduction #

Nuclear include an Integrated Development Environment for coding DSPs with FAUST, designing custom engine GUIs and visually program the processor graph. The IDE will be developed in Python.

We're taking a serious look at using [FlowCanvas](http://wiki.drobilla.net/FlowCanvas) as an easy method of drawing a graph-based visual programming GUI.

The IDE will come with a standard library of [DSP blocks](AtomicNodes.md).

**NOTE:** I decided to remove the proposed GUI mock-up, on several bases, not the least of which that the ideas I had in mind would make it too difficult for third parties to contribute new nodes to the program. I'm going back to rethink my design. I do, however, still like one idea:

  * The ability to change the locations of the icons representing the inputs and outputs of each node, so as to allow the drawing of any signal flow in a logical fashion.

I have a different idea on how to pursue this.