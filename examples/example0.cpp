#include "common_handlers.hpp"
#include "exception_generators.hpp"

// This example shows the simplest usage for catching any unhandled exception thrown from inside main()
// If such an exception is thrown, then the program returns EXIT_FAILURE, otherwise it returns EXIT_SUCCESS.

#if 1

//////////////////////////////////////////////////////////////////////////
// Function-try-block style

int main() try // Function try block
{
   throw_std_exception();
	return EXIT_SUCCESS;
}
catch (...)
{
   return mittens::all_catcher(EXIT_FAILURE).handleException(); 
}

#else

//////////////////////////////////////////////////////////////////////////
// Internal try-catch block

int main()  
{
   try
   {
      throw_std_exception();
      return EXIT_SUCCESS;
   }
   catch (...)
   {
      return mittens::all_catcher(EXIT_FAILURE).handleException(); 
   }
   return EXIT_FAILURE; // should never reach here
}

#endif