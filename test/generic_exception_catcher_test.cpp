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
      auto allCatcher = GenericExceptionHandler<void, UnHandler<>>(EXIT_FAILURE, UnHandler<>()); // external handler
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
      auto stdExceptionCatcher = GenericExceptionHandler<std::exception, UnHandler<>>(EXIT_FAILURE, UnHandler<>()); // external handler
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
      auto allCatcher = GenericExceptionHandler<void, UnHandler<>>(EXIT_FAILURE, UnHandler<>()); // external handler
      auto stdExceptionCatcher = GenericExceptionHandler<std::exception, UnHandler<>>(EXIT_FAILURE, UnHandler<>()); // external handler
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
   auto allCatcher = generic_handler<void>(EXIT_FAILURE, UnHandler<>());
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

   auto stdExceptionCatcher = generic_handler<std::exception>(EXIT_FAILURE, UnHandler<>()); //
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
      auto catcher = generic_handler<void>(-1, stdExceptionCatcher);

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
