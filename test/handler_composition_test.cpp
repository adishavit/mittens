#include <boost/test/unit_test.hpp>

//////////////////////////////////////////////////////////////////////////
// Tested API
#include "common_handlers.hpp"

//////////////////////////////////////////////////////////////////////////
#include "exception_generators.hpp"


using namespace mittens;

BOOST_AUTO_TEST_SUITE(CatcherComposition_Tests)

BOOST_AUTO_TEST_CASE(CatcherComposition_works)
{
   auto catcher = all_catcher(-1, std_exception_handler(-2)); // external handler
   try
   {
      throw_std_exception();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(-2, catcher.handleException());
   }

   try
   {
      throw_int();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(-1, catcher.handleException());
   }
}

BOOST_AUTO_TEST_CASE(CatcherComposition_with_3_catchers_works)
{
   auto catcher = all_catcher(-1, std_exception_handler(-2, bad_alloc_handler(-3))); // external handler

   try
   {
      throw_bad_alloc();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(-3, catcher.handleException());
   }

   try
   {
      throw_std_exception();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(-2, catcher.handleException());
   }

   try
   {
      throw_int();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(-1, catcher.handleException());
   }
}

/*
BOOST_AUTO_TEST_CASE(CatcherComposition_with_custom_action_works)
{
   int hit=0;
   auto catcher = all_catcher(-1, std_exception_handler(-2, bad_alloc_handler(-3,[&hit](std::exception&){ ++hit; }), [&hit](std::exception&){ ++hit; }), [&hit](){ ++hit; }); // external handler

   try
   {
      throw_bad_alloc();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(0,hit);
      BOOST_CHECK_EQUAL(-3, catcher.handleException());
      BOOST_CHECK_EQUAL(1,hit);
   }

   try
   {
      throw_std_exception();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(1,hit);
      BOOST_CHECK_EQUAL(-2, catcher.handleException());
      BOOST_CHECK_EQUAL(2,hit);
   }

   try
   {
      throw_int();
   }
   catch (...)
   {
      BOOST_CHECK_EQUAL(2,hit);
      BOOST_CHECK_EQUAL(-1, catcher.handleException());
      BOOST_CHECK_EQUAL(3,hit);
   }
}

*/


BOOST_AUTO_TEST_SUITE_END()
