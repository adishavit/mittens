#include <Windows.h>
#include <comdef.h>
#include <iostream>
#include "common_handlers.hpp"
#include "exception_generators.hpp"

using namespace mittens;

// Example based on this blog post: http://blogs.msdn.com/b/vcblog/archive/2014/01/16/exception-boundaries.aspx

/*
   From the article:
      For discussion purposes, let's say that our library uses its own exception type internally for failures, 
      named my_hresult_error. In addition, our library makes use of new and delete, so we may also need to handle 
      std::bad_alloc at the boundary. We don't expect any exceptions other than these at the boundary, so 
      for all other exceptions, we want to immediately terminate because we do not know what the state of the system is. 
*/
struct my_hresult_error
{  
   my_hresult_error(HRESULT const& hres = E_FAIL): hres_(hres) {}
   HRESULT hresult() const { return hres_; } 
private:
   HRESULT hres_;
};

auto boundaryExceptionGuard = all_catcher(E_UNEXPECTED, [](){ std::terminate(); }, 
                                 generic_handler<my_hresult_error>(E_UNEXPECTED, [](my_hresult_error const& ex) { return ex.hresult();  },
                                    std_exception_handler(E_UNEXPECTED,
                                       bad_alloc_handler(E_OUTOFMEMORY))));

//////////////////////////////////////////////////////////////////////////
// First function
extern "C" HRESULT boundary_function() try         // Function try block
{
   throw_std_exception(); // ... code that throws ...
   return S_OK;
}
catch (...) { return boundaryExceptionGuard.handleException(); };


//////////////////////////////////////////////////////////////////////////
// Second function which uses the same handler
extern "C" HRESULT another_boundary_function() try // Function try block
{
   throw_bad_alloc(); // ... code that throws ...
   return S_OK;
}
catch (...) { return boundaryExceptionGuard.handleException(); };


//////////////////////////////////////////////////////////////////////////
// Third function which uses the same handler
extern "C" HRESULT yet_another_boundary_function() try // Function try block
{
   throw my_hresult_error(); 
}
catch (...) { return boundaryExceptionGuard.handleException(); };


//////////////////////////////////////////////////////////////////////////
int main() 
{
   using namespace std;

   cout << _com_error(boundary_function()).ErrorMessage() << endl;
   cout << _com_error(another_boundary_function()).ErrorMessage() << endl;   
   cout << _com_error(yet_another_boundary_function()).ErrorMessage() << endl;   
   
   return EXIT_SUCCESS;
}
