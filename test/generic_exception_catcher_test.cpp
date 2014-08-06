#include <boost/test/unit_test.hpp>

//////////////////////////////////////////////////////////////////////////
// Tested API
#include "generic_exception_catcher.hpp"


//////////////////////////////////////////////////////////////////////////
#include "exception_generators.hpp"


using namespace mittens;


BOOST_AUTO_TEST_SUITE(GenericExceptionCatcher_Tests)

BOOST_AUTO_TEST_CASE(GenericExceptionCatcher_first_test)
{
   {
      auto allCatcher = GenericExceptionCatcher<void, UnCatcher<>>(EXIT_FAILURE, UnCatcher<>()); // external handler
      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, allCatcher.handleException());
      }
   }

   {
      auto stdExceptionCatcher = GenericExceptionCatcher<std::exception, UnCatcher<>>(EXIT_FAILURE, UnCatcher<>()); // external handler
      try
      {
         throw_std_exception();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, stdExceptionCatcher.handleException());
      }
   }

   {
      auto allCatcher = GenericExceptionCatcher<void, UnCatcher<>>(EXIT_FAILURE, UnCatcher<>()); // external handler
      auto stdExceptionCatcher = GenericExceptionCatcher<std::exception, UnCatcher<>>(EXIT_FAILURE, UnCatcher<>()); // external handler
      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_THROW(stdExceptionCatcher.handleException(), int);
         BOOST_CHECK_EQUAL(EXIT_FAILURE, allCatcher.handleException());
      }
   }
}

BOOST_AUTO_TEST_CASE(generic_catcher_first_test)
{
   auto allCatcher = generic_catcher<void>(EXIT_FAILURE, UnCatcher<>());
   {
      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, allCatcher.handleException());
      }
   }

   auto stdExceptionCatcher = generic_catcher<std::exception>(EXIT_FAILURE, UnCatcher<>()); //
   {
      try
      {
         throw_std_exception();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, stdExceptionCatcher.handleException());
      }
   }

   {
      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_THROW(stdExceptionCatcher.handleException(), int);
         BOOST_CHECK_EQUAL(EXIT_FAILURE, allCatcher.handleException());
      }
   }

   {
      // composition test
      auto catcher = generic_catcher<void>(-1, stdExceptionCatcher);

      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(-1, catcher.handleException());
      }
   }


}


BOOST_AUTO_TEST_SUITE_END()
