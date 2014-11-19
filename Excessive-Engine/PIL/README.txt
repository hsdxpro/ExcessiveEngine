Platform Independence Layer.
Common functionality provided directly by the Operating System is wrapped by this library. Implementation is different for every OS, but the interface stays the same.


The following belongs here:

platform detection: plaftorm_detection.cmake
file system: would need an interface...
network transport layer: boost (update if better alternatives are found)


The following is deprecated or does NOT belong here:

collections and iterators: STL, not a subject of argument
atomic data types: - deprecated since C++11's <atomic>
hi-res timer: would need an interface... - use std::chrono instead!
threading library: stl built-in + intel tbb - does not belong here
graphics wrappers: would need an interface... - gets separate project/dll
physics/collision wrappers: would need an interface... - gets separate project/dll
