#pragma once
#include <exception>

// #ifdef _MSC_VER
// #include <afx.h>
// #endif

inline void throw_int(int val = 0)
{
   throw val;
}

inline void throw_bad_alloc()
{
   //int* myarray= new int[100000000];
   // just in case the new works
   //delete myarray;

   throw std::bad_alloc();
}

inline void throw_runtime_error()
{
   throw std::runtime_error("std::runtime_error!");
}

inline void throw_std_exception()
{
   throw std::logic_error("std::exception!");
}

inline void throw_seh_exception()
{
   *(int*)(0) = 0;
}

// inline void throw_AfxThrowMemoryException()
// {
//    AfxThrowMemoryException();
// }

inline void throw_terminate_signal()
{
   std::terminate();
}

