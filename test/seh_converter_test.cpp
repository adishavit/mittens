#include <iostream>
#include <typeinfo>
#include <boost/test/unit_test.hpp>

//////////////////////////////////////////////////////////////////////////
// Tested API
#include "seh_handlers.hpp"


//////////////////////////////////////////////////////////////////////////
#include "common_handlers.hpp"
#include "exception_generators.hpp"


using namespace mittens;

BOOST_AUTO_TEST_SUITE(SEHExceptionCatcher_Tests)

//////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(Internal_SEH_Conversion_works)
{
   auto catcher = std_exception_handler(-1, seh_to_std_exception_converter<int>()); // must pass fail-code type as template argument
   try
   {
      throw_std_exception();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(-1, catcher.handleException());
   }

   try
   {
      throw_seh_exception();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(-1, catcher.handleException());
   }
}

BOOST_AUTO_TEST_CASE(External_SEH_Conversion_works)
{
   auto catcher = seh_to_std_exception_converter(std_exception_handler(-1)); 
   try
   {
      throw_std_exception();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(-1, catcher.handleException());
   }

   try
   {
      throw_seh_exception();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(-1, catcher.handleException());
   }
}

BOOST_AUTO_TEST_CASE(External_SEH_Conversion_works_with_printing)
{
   auto catcher = seh_to_std_exception_converter(std_exception_handler(-1, [](std::exception&e){ std::cout << "Caught unhandled std::exception: " << e.what() << std::endl; })); 
   try
   {
      throw_std_exception();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(-1, catcher.handleException());
   }

   try
   {
      throw_seh_exception();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(-1, catcher.handleException());
   }
}


BOOST_AUTO_TEST_CASE(Test_that_GlobalSEHException_works)
{
   GlobalSEHExceptionConverter sehConverter;
   {
      auto catcher = std_exception_handler(EXIT_FAILURE); // default nest handler
      try
      {
         throw_seh_exception();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, catcher.handleException());
      }
   }

}


BOOST_AUTO_TEST_SUITE_END()
