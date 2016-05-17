Mittens ![icon](mittens_icon.png)
=======

Mittens is a small header-only library for uniform handling of exceptions, particularly along module boundaries. 

Robust software needs to be able to handle exceptions as gracefully as possible. This might include last-minute error logging, alternate notification mechanisms (e.g. via the JVM) etc. 

An unexpected exit out of `main()` or an exception escaping from a DLL, COM or JNI function call can cause severe havoc. These module boundaries are stress-points where escaped exceptions should be firmly handled.
Along dynamic module boundaries, in particular, we want uniform handling of exceptions from multiple C-like functions. This often requires a lot of copy-paste code since try-catch statements do not naturally compose nor can they be easily encapsulated.

Mittens is a generic header-only library that allows try-catch statement composition, ordering and nesting with customizable actions per exception type. These composite exception handlers can be defined once and used multiple times where needed. 

Tutorial
--------
#### Goodbye World, Hello Mittens

Here's a very simple program throwing an exception:
```
#include "common_handlers.hpp"

using namespace mittens;

int main() try
{
   throw std::runtime_error("Goodbye World, Hello Mittens!");
   return EXIT_SUCCESS;
}
catch (...)
{
   return all_catcher(EXIT_FAILURE).handleException(); 
}
```

Obviously, `main` throws an error. We use the pre-defined helper handler `mittens::all_catcher` to intercept the exception and return the `EXIT_FAILURE` return value.  
Here, `mittens::all_catcher` takes a single argument - the value to be returned when calling `handleException()`. 

The example also demonstrates the use of the lesser known [function try block](http://en.cppreference.com/w/cpp/language/function-try-block), surrounding the function body from the *outside*. The code will also work the same in the try-catch block was inside the function.

#### Don't just stand there

But what if we want to do something extra when an exception is thrown?  
We just throw in a lambda (pun intended):
```
// same as above...
catch (...)
{
   return all_catcher(EXIT_FAILURE, [](){ cout << "Oh My!"<< endl; })(); 
}
```
Here, the handler `mittens::all_catcher` takes a two arguments:
 1. The value (`EXIT_FAILURE`) to be returned when activated. 
 2. A lambda to be called before returning.  
Note also that instead of calling `.handleException();` we can use the empty function call `operator()`.

This is all very nice, but isn't all that different (nor much shorter nor clearer) from regular try-catch(...) blocks.
And what about handling multiple exception types?  

#### Multiple Exception Types

This is where Mitten's power shines. As claimed above, Mittens allows exception catcher composition.  
Let's see an example:

```
// same as above...
catch (...)
{
   return all_catcher             (-1, []()       { std::cerr << "Caught unhandled exception of unknown type!"   << std::endl; }, 
             std_exception_handler(-2, [](auto& e){ std::cerr << "Caught unhandled std::exception: " << e.what() << std::endl; },
                bad_alloc_handler (-3, [](auto& e){ std::cerr << "Caught unhandled std::bad_alloc: " << e.what() << std::endl; })))(); 
}
```
Here we use 3 nested pre-defined handlers `all_catcher`, `std_exception_handler` and `bad_alloc_handler`.  
The exceptions will be handles in the order of composition, i.e. inner-to-outer, so fall-through is naturally supported. Each of the handlers also (optionally) accepts its own lambda for customization.

These 3 lines of code replace this typical code:
```
// same as above...
catch (std::bad_alloc& e)
{
   std::cerr << "Caught unhandled std::bad_alloc: " << e.what() << std::endl;
   return -3;
}
catch (std::exception& e)
{
   std::cerr << "Caught unhandled std::exception: " << e.what() << std::endl;
   return -2;
}
catch (...)
{
   std::cerr << "Caught unhandled exception of unknown type!" << std::endl;
   return -1;
}
```

What if this was not `main()` but multiple exported DLL C-functions?  
You would need to duplicate the catch code above at the end of *each* function! If the exception handling policy is changed you had better make sure that you re-copy-paste everything at the end of each of these functions.

Our handler is an object that can be created a reused. This allows us to separate the exception handling code and policy from the site where the exception is caught.  
Define the (composite) handler once, and use it over and over anywhere it is needed:

```
  // somewhere in scope:
  auto myHandler = all_catcher          (-1, []()       { std::cerr << "Caught unhandled exception of unknown type!"   << std::endl; }, 
                   std_exception_handler(-2, [](auto& e){ std::cerr << "Caught unhandled std::exception: " << e.what() << std::endl; },
                   bad_alloc_handler    (-3, [](auto& e){ std::cerr << "Caught unhandled std::bad_alloc: " << e.what() << std::endl; })));

  // my functions 
  int f() try 
  {
    //...
  }
  catch (...)
  { return myHandler(); } // <<< always stays the same
  
  int g() try 
  {
    //...
  }
  catch (...)
  { return myHandler(); } // <<< always stays the same
  
  void h() try 
  {
    //...
  }
  catch (...)
  { myHandler(); }  // <<< always stays the same. NOTE: no return, ignore value
  
```  

Check out the examples folder for more examples of how Mittens can help you and more advanced usage. 
