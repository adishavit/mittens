mittens
=======

Mittens is a small header-only library for uniform handling of exceptions, particularly along module boundaries. 

Robust software needs to be able to handle exceptions as gracefully as possible. This might include last-minute error logging, alternate notification mechanisms (e.g. via the JVM) etc. 

An unexpected exit out of main() or an exception escaping from a DLL, COM or JNI function call can cause severe havoc. These module boundaries are stress-points where escaped exceptions should be firmly handled.
Along dynamic module boundaries, in particular, we want uniform handling of exceptions from multiple C-like functions. This often requires a lot of copy-paste code since try-catch statements do not naturally compose nor can be easily encapsulated.

Mittens is a generic header-only library that allows try-catch statement composition, ordering and nesting with customizable actions per exception type. These composite exception handlers can be defined once and used multiple times where needed. 