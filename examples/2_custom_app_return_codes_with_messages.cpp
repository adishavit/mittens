#include <iostream>
#include "common_handlers.hpp"
#include "exception_generators.hpp"

int main() try // Function try block
{
   //throw_int();
   throw_std_exception();
   //throw_bad_alloc();

	return EXIT_SUCCESS;
}
catch (...)
{
   using namespace mittens;
   return   all_catcher(             -1, []()                { std::cout << "Caught unhandled exception of unknown type!"   << std::endl; }, 
               std_exception_handler(-2, [](auto& e){ std::cout << "Caught unhandled std::exception: " << e.what() << std::endl; },
                  bad_alloc_handler( -3, [](auto& e){ std::cout << "Caught unhandled std::bad_alloc: " << e.what() << std::endl; }))).
                     handleException(); 
}