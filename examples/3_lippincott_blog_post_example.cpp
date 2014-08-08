#include <iostream>
#include "common_handlers.hpp"

// Example based on this blog post Using a Lippincott Function for Centralized Exception Handling: 
//    http://cppsecrets.blogspot.ca/2013/12/using-lippincott-function-for.html
//
// Original code was here: http://ideone.com/m2ZfHN

#include <exception>

typedef enum foo_Result {
   FOO_OK,
   FOO_ERROR1,
   FOO_ERROR2,
   FOO_UNKNOWN
} foo_Result;

struct MyException1 { };
struct MyException2 { };


using namespace mittens;
auto lippincott = all_catcher(FOO_UNKNOWN, 
                     generic_handler<MyException2>(FOO_ERROR2, 
                        generic_handler<MyException1>(FOO_ERROR1)));

//////////////////////////

void Snafuscate(bool andDie)
{
   if (andDie)
   {
      throw MyException1();
   }
}

foo_Result foo_snafuscate(bool andDie)
{
   try
   {
      Snafuscate(andDie);
      return FOO_OK;
   }
   catch (...)
   {
      return lippincott();
   }
}

#include <iostream>

int main()
{
   foo_Result r1 = foo_snafuscate(false);
   if (r1 == FOO_OK) std::cout << "r1 == FOO_OK\n";

   foo_Result r2 = foo_snafuscate(true);
   if (r2 != FOO_OK) std::cout << "r2 != FOO_OK (r2 == " << r2 << ")\n";
}
