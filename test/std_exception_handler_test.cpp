#include <iostream>
#include <typeinfo>
#include <boost/test/unit_test.hpp>

//////////////////////////////////////////////////////////////////////////
// Tested API
#include "common_handlers.hpp"


//////////////////////////////////////////////////////////////////////////
#include "exception_generators.hpp"


using namespace mittens;

BOOST_AUTO_TEST_SUITE(StdExceptionCatcher_Tests)

//////////////////////////////////////////////////////////////////////////


BOOST_AUTO_TEST_CASE(Test_that_std_exception_catcher_works)
{
   {
      auto catcher = std_exception_handler(EXIT_FAILURE, UnHandler<int>());
      try
      {
         throw_std_exception();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, catcher.handleException());
      }
   }
   {
      auto catcher = std_exception_handler(EXIT_FAILURE); // default nest handler
      try
      {
         throw_std_exception();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, catcher.handleException());
      }
   }

}

BOOST_AUTO_TEST_CASE(Test_that_std_exception_catcher_with_differnt_fail_code_types_works)
{
   {
      auto catcher = std_exception_handler(false); // use bool fail code type

      try
      {
         throw_std_exception();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(false, catcher.handleException());
      }
   }

   {
      auto catcher = std_exception_handler(0.7); // use float fail code type

      try
      {
         throw_std_exception();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(0.7, catcher.handleException());
      }
   }
}

BOOST_AUTO_TEST_CASE(Internal_catch_handler_std_exception_catcher_works)
{
   {
      try
      {
         throw_std_exception();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, std_exception_handler(EXIT_FAILURE, UnHandler<int>()).handleException());
      }
   }
}


BOOST_AUTO_TEST_CASE(std_exception_catcher_does_not_catch_non_std_exception)
{
   {
      auto catcher = std_exception_handler(EXIT_FAILURE);
      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_THROW(catcher.handleException(), int); // should re-throw an 'int' exception
      }
   }
}


BOOST_AUTO_TEST_CASE(std_exception_catcher_catches_bad_alloc_exception)
{
   auto catcher = std_exception_handler(EXIT_FAILURE, UnHandler<int>());
   try
   {
      throw_bad_alloc();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(EXIT_FAILURE, catcher.handleException());
   }
}

BOOST_AUTO_TEST_CASE(std_exception_catcher_with_custom_action)
{
   int hit = 0;
   {
      auto catcher = std_exception_handler(EXIT_FAILURE, [&hit](std::exception&){++hit;}, UnHandler<int>());
      try
      {
         throw_std_exception();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(0, hit);
         BOOST_CHECK_EQUAL(EXIT_FAILURE, catcher.handleException());
         BOOST_CHECK_EQUAL(1, hit);
      }
   }
   {
      auto catcher = std_exception_handler(EXIT_FAILURE, [&hit](std::exception&){++hit;}); // with default nested handler
      try
      {
         throw_std_exception();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(1, hit);
         BOOST_CHECK_EQUAL(EXIT_FAILURE, catcher.handleException());
         BOOST_CHECK_EQUAL(2, hit);
      }
   }
}


BOOST_AUTO_TEST_SUITE_END()
