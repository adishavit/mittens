#include <boost/test/unit_test.hpp>

//////////////////////////////////////////////////////////////////////////
// Tested API
#include "common_handlers.hpp"


//////////////////////////////////////////////////////////////////////////
#include "exception_generators.hpp"


using namespace mittens;

BOOST_AUTO_TEST_SUITE(AllCatcher_Tests)


BOOST_AUTO_TEST_CASE(Test_that_all_catcher_works)
{
   {
      auto catcher = all_catcher(EXIT_FAILURE, UnHandler<int>());
      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, catcher.handleException());
      }
   }
   {
      auto catcher = all_catcher(EXIT_FAILURE); // default nest handler
      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, catcher.handleException());
      }
   }

}

BOOST_AUTO_TEST_CASE(Test_that_all_catcher_with_differnt_fail_code_types_works)
{
   {
      auto catcher = all_catcher(false); // use bool fail code type

      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(false, catcher.handleException());
      }
   }

   {
      auto catcher = all_catcher(0.7); // use float fail code type

      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(0.7, catcher.handleException());
      }
   }
}

BOOST_AUTO_TEST_CASE(Internal_catch_handler_all_catcher_works)
{
   {
      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, all_catcher(EXIT_FAILURE, UnHandler<int>()).handleException());
      }
   }
}

BOOST_AUTO_TEST_SUITE_END()
