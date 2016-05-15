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

BOOST_AUTO_TEST_CASE(CatcherComposition_with_custom_action_works)
{
   int hit=0;
   auto catcher = all_catcher(             -1, [&hit](){ ++hit; }, 
                     std_exception_handler(-2, [&hit](std::exception&){ ++hit; }, 
                        bad_alloc_handler( -3, [&hit](std::bad_alloc&){ ++hit; }, 
                           UnHandler<>()))); 

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


BOOST_AUTO_TEST_CASE(Catch_internal_CatcherComposition_with_custom_action_works)
{
   int hit=0;
   for (int i=0; i<3; ++i)
   {
      try
      {
         switch(i)
         {
         case 0: throw_bad_alloc();
         case 1: throw_std_exception();
         case 2: throw_int();
         }
      }
      catch (...)
      {
         auto catcher = all_catcher(             2, [&hit](){ ++hit; }, 
                           std_exception_handler(1, [&hit](std::exception&){ ++hit; }, 
                              bad_alloc_handler( 0, [&hit](std::bad_alloc&){ ++hit; }, 
                                 UnHandler<>()))); 

         BOOST_CHECK_EQUAL(i,hit);
         BOOST_CHECK_EQUAL(i, catcher.handleException());
         BOOST_CHECK_EQUAL(i+1,hit);
      }
   }
}




BOOST_AUTO_TEST_SUITE_END()
