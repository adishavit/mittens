#include <boost/test/unit_test.hpp>

//////////////////////////////////////////////////////////////////////////
// Tested API
#include "common_catchers.hpp"


//////////////////////////////////////////////////////////////////////////
#include "exception_generators.hpp"


using namespace mittens;

BOOST_AUTO_TEST_SUITE(BadAllocCatcher_Tests)



   //////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(Test_that_bad_alloc_catcher_works)
{
   {
      auto catcher = bad_alloc_catcher(EXIT_FAILURE, UnCatcher<int>());
      try
      {
         throw_bad_alloc();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, catcher.handleException());
      }
   }
   {
      auto catcher = bad_alloc_catcher(EXIT_FAILURE); // default nest handler
      try
      {
         throw_bad_alloc();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, catcher.handleException());
      }
   }

}

BOOST_AUTO_TEST_CASE(bad_alloc_catcher_does_not_catch_non_bad_alloc_exception)
{
   {
      auto catcher = bad_alloc_catcher(EXIT_FAILURE);
      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_THROW(catcher.handleException(), int); // should re-throw an 'int' exception
      }
   }
   {
      auto catcher = bad_alloc_catcher(EXIT_FAILURE);
      try
      {
         throw_std_exception();
      }
      catch (...)
      {
         BOOST_CHECK_THROW(catcher.handleException(), std::exception); // should re-throw an 'int' exception
      }
   }
}


BOOST_AUTO_TEST_SUITE_END()
