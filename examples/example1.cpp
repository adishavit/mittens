#include "common_handlers.hpp"
#include "exception_generators.hpp"

int main() try // Function try block
{
   throw_std_exception();
	return EXIT_SUCCESS;
}
catch (...)
{
   using namespace mittens;
   return all_catcher(-1, std_exception_handler(-2, bad_alloc_handler(-3))).handleException(); 
}