![icon](mittens_icon.png) Mittens 
=======

Mittens is a cross-platform, C++ header-only library for centralized handling of exceptions, particularly along module boundaries. 

Robust software needs to be able to handle exceptions as gracefully as possible. This might include last-minute error logging, alternate notification mechanisms (e.g. via the JVM) etc. 

An unexpected exit out of `main()` or an exception escaping from a DLL, COM or JNI function call can cause severe havoc. Module boundaries are stress-points where escaped exceptions should be firmly handled.  
Along dynamic module boundaries, in particular, we want uniform handling of exceptions from multiple C-like functions. This often requires a lot of copy-paste code since try-catch statements do not naturally compose nor can they be easily encapsulated.

Mittens is a generic header-only library that allows try-catch statement composition, ordering and nesting with customizable actions per exception type. These composite exception handlers can be defined once and reused multiple times where needed. 

![icon](mittens_icon.png) TL;DR
-----
##### Features
- Centralized, uniform, maintainable, consistent and composable exception handling;
- Support both portable cross-platform and platform-specific code;
- Write robust, well ordered, exception-type-specific custom code;
- Particularly useful along module boundaries: e.g command-line applications and dynamically loaded libraries (DLLs, shared-object `.so`, Android JNI, etc.) 

##### Show me the code!
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

##### Integration
- Single header file `generic_exception_handler.hpp` + optional helper header (`common_handlers.hpp`).
- Cross platform: Windows, Linux OSX;
- Latest version and examples use C++11/14 features like `auto` and [polymorphic] lambdas but the core code should be easily downgradable to older compilers (e.g. VS2010);  
- No external dependencies (but unit tests use Boost.Test);
- BSD License 

![icon](mittens_icon.png) Tutorial
--------
#### Goodbye World, Hello Mittens

Here's a very simple program throwing an exception:
```cpp
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

Our `main` throws an exception. We use the pre-defined helper handler `mittens::all_catcher` to intercept the exception and return the `EXIT_FAILURE` return value.  
Here, `mittens::all_catcher` takes a single argument - the value to be returned when calling `handleException()`. 

&#10148; The example also demonstrates the use of the lesser known [function try block](http://en.cppreference.com/w/cpp/language/function-try-block), surrounding the function body from the *outside*. The code will also work the same if the try-catch block was inside the function.

#### Don't just stand there

But what if we want to do something extra when an exception is thrown?  
We just throw in a lambda (pun intended):
```cpp
// same as above...
catch (...)
{
   return all_catcher(EXIT_FAILURE, [](){ cout << "Oh My!"<< endl; })(); 
}
```
Here, the handler `mittens::all_catcher` takes a *two* arguments:
 1. The value (`EXIT_FAILURE`) to be returned when activated. 
 2. A lambda to be called before returning.  

&#10148; Note also, that instead of calling `.handleException();` we can use the function call `operator()`.

This is all very nice, but it isn't all that different (nor much shorter nor clearer) from regular try-catch(...) blocks.  
And what about handling multiple exception types?  

#### Handling Multiple Exception Types

This is where Mittens' true power begins to manifest itself. As claimed above, Mittens allows exception catcher composition.  
Let's see an example:

```cpp
// same as above...
catch (...)
{
   return all_catcher             (-1, []()       { std::cerr << "Caught unhandled exception of unknown type!"   << std::endl; }, 
             std_exception_handler(-2, [](auto& e){ std::cerr << "Caught unhandled std::exception: " << e.what() << std::endl; },
                bad_alloc_handler (-3, [](auto& e){ std::cerr << "Caught unhandled std::bad_alloc: " << e.what() << std::endl; })))(); 
}
```
Here we use 3 nested pre-defined handlers `all_catcher`, `std_exception_handler` and `bad_alloc_handler`.  
The exceptions will be handled in the order of composition, i.e. inner-to-outer, so fall-through is naturally supported. Each of the handlers also (optionally) accepts its own lambda for customization.   
The *lambdas* for `std_exception_handler` and `bad_alloc_handler` take the exception type as parameters.  
&#10148; Where polymorphic lambdas are available, simply use `auto`. 

In this form, the first 2 handlers (`all_catcher` and `std_exception_handler`) take *three* arguments:  
 1. The value to be returned when activated. 
 2. A lambda to be called before returning.
 3. Another (nested) handler  

These 3 lines of code replace this typical code:
```cpp
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

#### The DRY Principle

What if this was not `main()` but multiple exported DLL C-functions?  

You would need to duplicate the catch code above at the end of *each* function!  
When the exception handling policy is changed (e.g. when new exception types are added or different logging is needed), you had better make sure that you re-copy-paste everything properly at the end of each one of these functions.

Our handler is an object that can be created a reused.  
This allows us to separate the exception handling code and policy from the site where the exception is caught.  
Define the (composite) handler once, and use it over and over anywhere it is needed:

```cpp
  // somewhere in scope:
  auto myHandler = all_catcher          (-1, []()       { std::cerr << "Caught unhandled exception of unknown type!"   << std::endl; }, 
                   std_exception_handler(-2, [](auto& e){ std::cerr << "Caught unhandled std::exception: " << e.what() << std::endl; },
                   bad_alloc_handler    (-3, [](auto& e){ std::cerr << "Caught unhandled std::bad_alloc: " << e.what() << std::endl; })));

  // my functions 
  int f() try 
  { /* ... */ }
  catch (...)
  { return myHandler(); } // <<< always stays the same
  
  int g() try 
  { /* ... */ }
  catch (...)
  { return myHandler(); } // <<< always stays the same
  
  void h() try 
  { /* ... */ }
  catch (...)
  { myHandler(); }  // <<< always stays the same. NOTE: no return, ignore value
  
```  
When the exception handling logic changes, all we need to do is update the `myHandler` definition. The rest of the code remains unchanged.  
&#10148; Here, `myHandler()` (using the `operator()` syntax) is a naturally looking alias for `myHandler.handleException()`.  

#### Custom Exception Types
Handling custom exception types is very similar to pre-defined types.  
We parametrize `mittens::generic_handler<>` with our custom type:
```cpp
struct MyException: std::exception
{
  // ctor and more code ...
  int foo();
}

int main() try
{
   throw MyException("Goodbye World, Hello Mittens!");
   return EXIT_SUCCESS;
}
catch (...)
{
   return generic_handler<void>          (-1, []()       { std::cerr << "Caught unexpected exception!" << std::endl; }, 
          generic_handler<std::exception>(-2, [](auto& e){ std::cerr << "Caught std::exception: "      << e.what() << std::endl; },
          generic_handler<MyException>   (-3, [](auto& e){ std::cerr << "Caught MyException: "         << e.foo() << std::endl; }, 
          generic_handler<int>           (-4, [](auto& e){ std::cerr << "Caught thrown int: "          << e << std::endl; })))(); 
}
```
In this snippet we separately handle exceptions of the following types and in this order: `int`, `MyException`, `std::exception` and then any other type. When catching `MyException` we can naturally use the `MyException::foo()` method as expected.  

The pre-defined types mentioned previously are simply specialized versions of `generic_handler<>`.  
- `std_exception_handler` is an alias for `generic_handler<std::exception>`   
- `all_catcher` is an alias for `generic_handler<void>`!

&#10148; Note that since `void` is the only type that *cannot* be thrown, it is a perfect choice to indicate the *type-less* catch-all `(...)`.     


#### Dependent Return Values
`/* ... to-be-described ... */`
#### Logging and Rethrowing
`/* ... to-be-described ... */`
#### Advanced Usage
```
/* ... to-be-described ...
- Platform specific examples ...
- Type Conversions (JVM JNI) ...  
*/
```

#### Epilogue
- Mittens grew out of the need to robustly and consistently handle exceptions across module boundaries in Windows DLLs and Android JNIs. 
- The first version was written in 2013. This repo is a complete rewrite to allow for much more genericity.
- Although the design is very different, Mittens was inspired by Matthew Wilson's Quality Matters articles ([QM#6](http://accu.org/index.php/journals/1706), [QM#7](http://accu.org/index.php/articles/1868)).  
- Check out the examples folder for more examples of how Mittens can help you and more advanced usage.
- Comments, discussions, questions, ideas and PRs are most welcome!



