#pragma once

#include <iostream>
#include <generic_exception_handler.hpp>

#include <windows.h>
#include <eh.h>


namespace
{
   // from : http://stackoverflow.com/questions/3523716/is-there-a-function-to-convert-exception-pointers-struct-to-a-string
   //        http://msdn.microsoft.com/en-us/library/windows/desktop/aa363082(v=vs.85).aspx
   static const char* seDescription( DWORD& code )
   {
      switch( code ) 
      {
      case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION - The thread tried to read from or write to a virtual address for which it does not have the appropriate access.";
      case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED - The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking.";
      case EXCEPTION_BREAKPOINT:               return "EXCEPTION_BREAKPOINT - A breakpoint was encountered.";
      case EXCEPTION_DATATYPE_MISALIGNMENT:    return "EXCEPTION_DATATYPE_MISALIGNMENT - The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on.";
      case EXCEPTION_FLT_DENORMAL_OPERAND:     return "EXCEPTION_FLT_DENORMAL_OPERAND - One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value.";
      case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "EXCEPTION_FLT_DIVIDE_BY_ZERO - The thread tried to divide a floating-point value by a floating-point divisor of zero.";
      case EXCEPTION_FLT_INEXACT_RESULT:       return "EXCEPTION_FLT_INEXACT_RESULT - The result of a floating-point operation cannot be represented exactly as a decimal fraction.";
      case EXCEPTION_FLT_INVALID_OPERATION:    return "EXCEPTION_FLT_INVALID_OPERATION - This exception represents any floating-point exception not included in this list.";
      case EXCEPTION_FLT_OVERFLOW:             return "EXCEPTION_FLT_OVERFLOW - The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.";
      case EXCEPTION_FLT_STACK_CHECK:          return "EXCEPTION_FLT_STACK_CHECK - The stack overflowed or underflowed as the result of a floating-point operation.";
      case EXCEPTION_FLT_UNDERFLOW:            return "EXCEPTION_FLT_UNDERFLOW - The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.";
      case EXCEPTION_ILLEGAL_INSTRUCTION:      return "EXCEPTION_ILLEGAL_INSTRUCTION - The thread tried to execute an invalid instruction.";
      case EXCEPTION_IN_PAGE_ERROR:            return "EXCEPTION_IN_PAGE_ERROR - The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network.";
      case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "EXCEPTION_INT_DIVIDE_BY_ZERO - The thread tried to divide an integer value by an integer divisor of zero.";
      case EXCEPTION_INT_OVERFLOW:             return "EXCEPTION_INT_OVERFLOW - The result of an integer operation caused a carry out of the most significant bit of the result.";
      case EXCEPTION_INVALID_DISPOSITION:      return "EXCEPTION_INVALID_DISPOSITION - An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception.";
      case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION - The thread tried to continue execution after a noncontinuable exception occurred.";
      case EXCEPTION_PRIV_INSTRUCTION:         return "EXCEPTION_PRIV_INSTRUCTION - The thread tried to execute an instruction whose operation is not allowed in the current machine mode.";
      case EXCEPTION_SINGLE_STEP:              return "EXCEPTION_SINGLE_STEP - A trace trap or other single-instruction mechanism signaled that one instruction has been executed.";
      case EXCEPTION_STACK_OVERFLOW:           return "EXCEPTION_STACK_OVERFLOW - The thread used up its stack.";
      default: return "UNKNOWN STRUCTURED EXCEPTION" ;
      }
   }
}

namespace mittens
{
   class GlobalSEHExceptionConverter
   {
   public:
      GlobalSEHExceptionConverter()
      {  _set_se_translator(trans_func); } // calling _set_se_translator() requires /EHa

      static void trans_func( unsigned int u, EXCEPTION_POINTERS* pExp )
      {
         if (pExp && pExp->ExceptionRecord)
            throw std::runtime_error(std::string("SEH Exception: '") + seDescription(pExp->ExceptionRecord->ExceptionCode) + "'");

         throw std::runtime_error("Unknown SEH Exception thrown!");
      }
   };


   // For use in nested initialization
   template <typename NestedHandler>
   class NestedSEHExceptionConverter: public GlobalSEHExceptionConverter
   {
   public:
      typedef typename NestedHandler::FailCodeType FailCodeType;

      NestedSEHExceptionConverter(NestedHandler const& nestedHandler):
         nestedHandler(nestedHandler)
      {} 

      FailCodeType handleException() { return nestedHandler(); }
      FailCodeType operator()() { return handleException(); }

   private:
      NestedHandler nestedHandler;
   };

   template <typename NestedHandler>
   inline NestedSEHExceptionConverter<NestedHandler> seh_to_std_exception_converter(NestedHandler const& nestedHandler)
   {  return NestedSEHExceptionConverter<NestedHandler>(nestedHandler); }

   template <typename FailCodeType>
   inline NestedSEHExceptionConverter<UnHandler<FailCodeType>> seh_to_std_exception_converter()
   {  return NestedSEHExceptionConverter<UnHandler<FailCodeType>>(UnHandler<FailCodeType>()); }

}
