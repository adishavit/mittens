#include <iostream>
#include "common_handlers.hpp"
#include "exception_generators.hpp"

//////////////////////////////////////////////////////////////////////////
// Define exception handler for logging only.
// It will catch any exception, log that it happened, and then throw a std::runtime_error instead for 
// external handlers to catch and handle.
auto loggingHandler = mittens::all_catcher( 0xBad, []() 
   { 
      std::cout << "LOG: Caught exception!"   << std::endl; 
      throw std::runtime_error("Unknown exception converted to std::runtime_error()!"); 
   }, 
   false); // Prevent (default) custom-action exception-suppression. Allow std::runtime_error to propogate.

void f() try
{
   throw_int();
}
catch (...)
{
   loggingHandler.handleException(); 
}


//////////////////////////////////////////////////////////////////////////

int main() try // Function try block
{
   f();
	return EXIT_SUCCESS;
}
catch (...)
{
   // Define a separate exception handler un-related to 'loggingHandler' to handle various exceptions.
   using namespace mittens;
   return   all_catcher(             -1, []()                        { std::cout << "Caught unhandled exception of unknown type!"   << std::endl; }, 
               std_exception_handler(-2, [](std::exception&e)        { std::cout << "Caught unhandled std::exception: " << e.what() << std::endl; },
                  bad_alloc_handler( 0xBadA110c, [](std::bad_alloc&e){ std::cout << "Caught unhandled std::bad_alloc: " << e.what() << std::endl; }))).
                     handleException(); 
}
