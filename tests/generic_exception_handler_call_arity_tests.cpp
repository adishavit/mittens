#include <boost/test/unit_test.hpp>
#include "common_handlers.hpp"

//////////////////////////////////////////////////////////////////////////
// Tested API
#include "generic_exception_handler.hpp"


//////////////////////////////////////////////////////////////////////////
#include "exception_generators.hpp"


using namespace mittens;


BOOST_AUTO_TEST_SUITE(GenericExceptionCatcher_Call_Arity_Tests)


BOOST_AUTO_TEST_CASE(generic_catcher_1_param)
{
   {
      auto allCatcher = generic_handler<void>(EXIT_FAILURE);
      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(EXIT_FAILURE, allCatcher.handleException());
      }
   }
}

BOOST_AUTO_TEST_CASE(generic_catcher_2_param_with_custom_action)
{
   int hit=0;
   {
      auto allCatcher = generic_handler<void>(EXIT_FAILURE, [&hit](){ ++hit; });
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
}

BOOST_AUTO_TEST_CASE(generic_catcher_2_param_with_nested_handler)
{
   {
      auto stdCatcher = generic_handler<int>(-2);
      auto catcher = generic_handler<void>(EXIT_FAILURE, stdCatcher);
      {
         try
         {
            throw_int();
         }
         catch (...)
         {
            BOOST_CHECK_EQUAL(-2, catcher.handleException());
         }
      }
   }
}


BOOST_AUTO_TEST_CASE(generic_catcher_2_param_with_multiple_nested_handlers_0)
{
   using namespace mittens;

   {
      auto catcher = generic_handler<void>(-1,
                        generic_handler<int>(-2,
                           generic_handler<float>(-3)));

      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(-2, catcher.handleException());
      }
   }

   {
      struct MyException1 { };
      auto catcher = all_catcher(-1,
                        generic_handler<int>(-2,
                           generic_handler<MyException1>(-3)));

      try
      {
         throw_int();
      }
      catch (...)
      {
         BOOST_CHECK_EQUAL(-2, catcher.handleException());
      }
   }
}



BOOST_AUTO_TEST_SUITE_END()
