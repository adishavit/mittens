#pragma once

#include <iostream>
#include <type_traits> // for std::enable_if<> and std::is_same<>

namespace mittens
{
   //////////////////////////////////////////////////////////////////////////

   template <typename FailCodeType_ = int>
   class UnHandler
   {
   public:
      typedef FailCodeType_ FailCodeType;

      FailCodeType handleException()
      {
         throw; // Don't catch anything. Re-throw any current exception
      }
   };


   //////////////////////////////////////////////////////////////////////////
   // Helper utils
   namespace
   {
      template <bool v> struct Bool2Type { static const bool value = v; };  // anonymous helper function
   }

   // Helper default no-action type
   template <typename ExceptionType = void>
   struct DefaultNoAction
   {  void operator()(ExceptionType&){} };

   template <>
   struct DefaultNoAction<void>
   {  void operator()(){} };

   //////////////////////////////////////////////////////////////////////////

   template <typename ExceptionType, typename NestedHandler, typename Callable = DefaultNoAction<ExceptionType> >
   class GenericExceptionHandler
   {
   public:
      typedef typename NestedHandler::FailCodeType FailCodeType;

      GenericExceptionHandler(FailCodeType failCode, NestedHandler const& nestedHandler, Callable customAction = Callable()):
         failCode_(failCode),
         nestedHandler(nestedHandler),
         customAction_(customAction)
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
         catch (ExceptionType& e)
         {
            try { customAction_(e); } catch (...) {} // Run custom action. Force it no-throw by suppressing any exceptions
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
            try { customAction_(); } catch (...) {} // Run custom action. Force it no-throw by suppressing any exceptions
            return failCode_;
         }
      }

   private:
      FailCodeType failCode_;
      NestedHandler nestedHandler;
      Callable customAction_; 
   };


   template <typename ExceptionType, typename NestedHandler, typename Callable>
   inline GenericExceptionHandler<ExceptionType, NestedHandler, Callable> generic_handler(typename NestedHandler::FailCodeType failCode, NestedHandler const& nestedHandler, Callable customAction)
   {  return GenericExceptionHandler<ExceptionType, NestedHandler, Callable>(failCode, nestedHandler, customAction); }


   template <typename ExceptionType, typename NestedHandler>
   inline GenericExceptionHandler<ExceptionType, NestedHandler, DefaultNoAction<ExceptionType>> generic_handler(typename NestedHandler::FailCodeType failCode, NestedHandler const& nestedHandler)
   {  return GenericExceptionHandler<ExceptionType, NestedHandler, DefaultNoAction<ExceptionType>>(failCode, nestedHandler, DefaultNoAction<ExceptionType>()); }

}
