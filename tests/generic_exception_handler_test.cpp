#include <boost/test/unit_test.hpp>

//////////////////////////////////////////////////////////////////////////
// Tested API
#include "generic_exception_handler.hpp"


//////////////////////////////////////////////////////////////////////////
#include "exception_generators.hpp"


using namespace mittens;


BOOST_AUTO_TEST_SUITE(GenericExceptionCatcher_Tests)

BOOST_AUTO_TEST_CASE(GenericExceptionCatcher_first_test)
{
   auto allCatcher = GenericExceptionHandler<void, UnHandler<>>(EXIT_FAILURE, UnHandler<>()); 
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

   auto stdExceptionCatcher = GenericExceptionHandler<std::exception, UnHandler<>>(EXIT_FAILURE, UnHandler<>()); 
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
}


BOOST_AUTO_TEST_CASE(generic_catcher_simple_tests)
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

BOOST_AUTO_TEST_CASE(generic_catcher_with_custom_action)
{
   int hit=0;
   {
      auto allCatcher = generic_handler<void>(EXIT_FAILURE, [&hit](){ ++hit; }, UnHandler<>());
      {
         try
         {
            throw_int();
         }
         catch (...)
         {
            BOOST_CHECK_EQUAL(0,hit);
            BOOST_CHECK_EQUAL(EXIT_FAILURE, allCatcher.handleException());
            BOOST_CHECK_EQUAL(1,hit);
         }
      }
   }

   {
      auto stdExceptionCatcher = generic_handler<std::exception>(EXIT_FAILURE, [&hit](std::exception&){ ++hit; }, UnHandler<>());
      {
         try
         {
            throw_std_exception();
         }
         catch (...)
         {
            BOOST_CHECK_EQUAL(1,hit);
            BOOST_CHECK_EQUAL(EXIT_FAILURE, stdExceptionCatcher.handleException());
            BOOST_CHECK_EQUAL(2,hit);
         }
      }
   }
}

BOOST_AUTO_TEST_CASE(generic_catcher_with_custom_action_which_return_fail_code)
{
   int DEFAULT_RETURN_VALUE  = 0;
   {
      int EXPECTED_RETURN_VALUE = 1;
      auto allCatcher = generic_handler<void>(DEFAULT_RETURN_VALUE, [&](){ return EXPECTED_RETURN_VALUE; });
      {
         try
         {
            throw_int();
         }
         catch (...)
         {
            BOOST_CHECK_EQUAL(EXPECTED_RETURN_VALUE, allCatcher.handleException());
         }
      }
   }
   {
      auto allCatcher = generic_handler<void>(DEFAULT_RETURN_VALUE, [&](){ return 2.5; }); // labda retunrs a double != int, so default value should return
      {
         try
         {
            throw_int();
         }
         catch (...)
         {
            BOOST_CHECK_EQUAL(DEFAULT_RETURN_VALUE, allCatcher.handleException());
         }
      }
   }
   {
      int EXPECTED_RETURN_VALUE = 1;
      auto intCatcher = generic_handler<int>(DEFAULT_RETURN_VALUE, [&](int&){ return EXPECTED_RETURN_VALUE; });
      {
         try
         {
            throw_int();
         }
         catch (...)
         {
            BOOST_CHECK_EQUAL(EXPECTED_RETURN_VALUE, intCatcher.handleException());
         }
      }
   }
   {
      auto intCatcher = generic_handler<int>(DEFAULT_RETURN_VALUE, [&](int&){ return 2.5; }); // labda retunrs a double != int, so default value should return
      {
         try
         {
            throw_int();
         }
         catch (...)
         {
            BOOST_CHECK_EQUAL(DEFAULT_RETURN_VALUE, intCatcher.handleException());
         }
      }
   }
   {
      auto intCatcher = generic_handler<int>(0.1, [&](int&) { return 2.5; }); // labda retunrs a double != int, so default value should return
      {
         try
         {
            throw_int();
         }
         catch (...)
         {
            BOOST_CHECK_EQUAL(2.5, intCatcher.handleException());
         }
      }
   }
}

BOOST_AUTO_TEST_CASE(GenericExceptionCatcher_paren_operator_test)
{
   auto allCatcher = generic_handler<void>(EXIT_FAILURE); 
   {
      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, allCatcher());
      }
   }
}

BOOST_AUTO_TEST_SUITE_END()
