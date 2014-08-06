#pragma once

#include <iostream>
#include <type_traits> // for std::enable_if<> and std::is_same<>

namespace mittens
{
   //////////////////////////////////////////////////////////////////////////

   template <typename FailCodeType_ = int>
   class UnCatcher
   {
   public:
      typedef FailCodeType_ FailCodeType;

      FailCodeType handleException()
      {
         throw; // Don't catch anything. Re-throw any current exception
      }
   };


   //////////////////////////////////////////////////////////////////////////

   namespace
   {
      template <bool v> struct Bool2Type { static const bool value = v; };  // anonymous helper function
   }

   //////////////////////////////////////////////////////////////////////////

   template <typename ExceptionType, typename NestedHandler>
   class GenericExceptionCatcher
   {
   public:
      typedef typename NestedHandler::FailCodeType FailCodeType;

      GenericExceptionCatcher(FailCodeType failCode, NestedHandler const& nestedHandler):
         failCode_(failCode),
         nestedHandler(nestedHandler)
      {}

      FailCodeType handleException() { return handleException_(Bool2Type<std::is_same<ExceptionType, void>::value>()); }

   private:
      FailCodeType handleException_(Bool2Type<false>)
      {
         // Handle the case of ExceptionType != void
         try
         {
            return nestedHandler.handleException();
         }
         catch (ExceptionType& /*e*/)
         {
            //std::cout << "Caught std::exception: " << e.what() << std::endl;
            return failCode_;
         }
      }

      FailCodeType handleException_(Bool2Type<true>)
      {
         // Handle the case of ExceptionType == void: Interpret this as catch-all: '...'
         try
         {
            return nestedHandler.handleException();
         }
         catch (...)
         {
            //std::cout << "Caught std::exception: " << e.what() << std::endl;
            return failCode_;
         }
      }

   private:
      FailCodeType failCode_;
      NestedHandler nestedHandler;
   };


   template <typename ExceptionType, typename NestedHandler>
   inline GenericExceptionCatcher<ExceptionType, NestedHandler> generic_catcher(typename NestedHandler::FailCodeType failCode, NestedHandler const& nestedHandler)
   {  return GenericExceptionCatcher<ExceptionType, NestedHandler>(failCode, nestedHandler); }

}
