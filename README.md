Heat master
==========

<b>This worker is mostly the work of Baptiste Pauget, though I (Théophile Bastian) contributed a little, mostly to the networking part.</b>

Worker software for parallel simulation of a discrete heat equation/propagation equation.

Its role is to compute and iterate the heat equation over a data set given by the master software, and send it back to the master. It could be well improved by limiting the frequency of full-copy of the results to the master.

This worker supports multithreading to allow fast calculation on multi-core computers.

Requirements
----------

Qt, Qt networking, and at least two computers in the same LAN :)

Compiling
--------

```bash
$ qmake
$ make
```

Running 
------

Simply launch the program, enter the master computer's IP address, and connect!
