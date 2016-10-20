# sim_cache

This is a cache simulator with support for various cache sizes and geometries, as well as multiple replacement policies. Currently, only an L1 cache is implemented. The cache object itself is generic enough to be easily converted to an L2 or L3 cache, but this may require minor tweaking depending on whether you would prefer an exclusive or inclusive cache (or neither!).

An input file with a list of address accesses may be read by the simulator, and will output hit/miss rates based on the parameters given.

This particular implementation was tested on a trace file of 2 million accesses.
