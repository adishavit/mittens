#include <iostream>
#include "common_handlers.hpp"
#include "exception_generators.hpp"
#include <ios>

struct JNIEnv
{
   int foo = 42;
};

auto getHandler(JNIEnv* jvm)
{
   using namespace mittens;
   return all_catcher(false, [jvm]()
          { 
//              __android_log_print(ANDROID_LOG_ERROR,"MY_JNI","Unknown exception"); 
//              throw_JavaException(jvm, "Unknown exception");
             std::cerr << jvm->foo << std::endl;
          },
          std_exception_handler(false, [jvm](auto &e) 
          { 
//              __android_log_print(ANDROID_LOG_ERROR,"MY_JNI","std::exception exception caught: '%s'",e.what());
//              throw_JavaException(jvm, e.what());
             std::cerr << jvm->foo << std::endl;
          }));
};

bool f(JNIEnv* jvm) try
{
   throw_int();
   return true;
}
catch (...)
{
   return getHandler(jvm).handleException(); 
}

bool g(JNIEnv* jvm) try
{
   throw_std_exception();
   return true;
}
catch (...)
{
   return getHandler(jvm).handleException();
}


//////////////////////////////////////////////////////////////////////////

int main()
{
   JNIEnv jvm;
   std::cout << std::boolalpha << f(&jvm) << std::endl << g(&jvm) << std::endl;
	return EXIT_SUCCESS;
}
