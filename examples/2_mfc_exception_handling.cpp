// Example to show how to handle MFC exception derived from CException
#define _AFXDLL

#include <afx.h>
#include <iostream>
#include "common_handlers.hpp"

int main() try // Function try block
{
   AfxThrowMemoryException();
   return EXIT_SUCCESS;
}
catch (...)
{
   using namespace mittens;
   return   all_catcher(-1, 
            std_exception_handler(-2, 
            generic_handler<CException*>(-3, [](CException* e) { e->Delete(); },            // Release CException object to prevent a leak            
            bad_alloc_handler(-4, 
            generic_handler<CMemoryException*>(-5, [](CMemoryException* e) { e->Delete(); } // Release CException object to prevent a leak
            ))))).handleException(); 
}
