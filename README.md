![icon](mittens_icon.png) Mittens 
=======

Mittens is a cross-platform, C++ header-only library for centralized handling of exceptions, particularly along module boundaries. 

Robust software needs to be able to handle exceptions as gracefully as possible. This might include last-minute error logging, alternate notification mechanisms (e.g. via the JVM) etc. 

An unexpected exit out of `main()` or an exception escaping from a DLL, COM or JNI function call can cause severe havoc. Module boundaries are stress-points where escaped exceptions should be firmly handled.  
Along dynamic module boundaries, in particular, we want uniform handling of exceptions from multiple C-like functions. This often requires a lot of copy-paste code since try-catch statements do not naturally compose nor can they be easily encapsulated.

Mittens is a generic header-only library that allows try-catch statement composition, ordering and nesting with customizable actions per exception type. These composite exception handlers can be defined once and reused multiple times where needed. 

![icon](mittens_icon.png) Overview
-----
#### Features
- Centralized, uniform, maintainable, consistent and composable exception handling;
- Support both portable cross-platform and platform-specific code;
- Write robust, well ordered, exception-type-specific custom code;
- Particularly useful along module boundaries: e.g command-line applications and dynamically loaded libraries (DLLs, shared-object `.so`, Android JNI, etc.) 

#### TL;DR: Show Me the Code!
```cpp
#include "common_handlers.hpp"

auto handleIt = mittens::all_catcher               (-1, []()       { std::cerr << "Caught exception of unknown type!"   << std::endl; }, 
                   mittens::std_exception_handler  (-2, [](auto& e){ std::cerr << "Caught std::exception: " << e.what() << std::endl; },
                      mittens::generic_handler<int>(-3, [](auto& e){ std::cerr << "Caught thrown `int` = " << e << std::endl; })));

int main() try
{
   throw std::runtime_error("Goodbye World, Hello Mittens!");
   return EXIT_SUCCESS;
}
catch (...) 
{  return handleIt(); }
```
Application prints: `Caught std::exception: Goodbye World, Hello Mittens!` and the app return value will be `-2`.

**See the [Tutorial](https://github.com/adishavit/mittens/wiki/The-Mittens-Tutorial) for more.**

#### Integration
- Single header file `generic_exception_handler.hpp` + optional helper header (`common_handlers.hpp`).
- Cross platform: Windows, Linux OSX;
- No external dependencies (but unit tests use Boost.Test);
- BSD License 

#### Epilogue
- Mittens grew out of the need to robustly and consistently handle exceptions across module boundaries in Windows DLLs and Android JNIs. 
- The first version was written in 2013. This repo is a complete rewrite to allow for much more genericity.
- Although the design is very different, Mittens was originally inspired by Matthew Wilson's Quality Matters articles ([QM#6](http://accu.org/index.php/journals/1706), [QM#7](http://accu.org/index.php/articles/1868)).  
- Check out the `examples` folder for more examples of how Mittens can help you and more advanced usage.
- Comments, discussions, questions, ideas and PRs are most welcome!



