#pragma once

#include <iostream>
#include <type_traits> // for std::is_same<>
#include <functional>
//#include <boost/utility/result_of.hpp>
#include <exception> // for std::is_same<>

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
      FailCodeType operator()() { return handleException(); }
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
      FailCodeType operator()() { return handleException(); }

   private:
      //////////////////////////////////////////////////////////////////////////
      // What follows is lots of code and compile-time trickery to work-around 
      // partial support for template specialization in VS2010
      FailCodeType handleException_(Bool2Type<false> exceptionTypeNotVoid)
      {
         // Handle the case of ExceptionType != void
         // This means 'e' can and needs to be passed to the custom-action (if the exception is caught)
         try
         {
            return nestedHandler();
         }
         catch (ExceptionType& e)
         {
            // If the return type of the custom action is the same as FailCodeType (as opposed to void or even less plausible something else)
            // then return the result of custom action as the fail code.
            auto shouldReturnActionResult = Bool2Type< std::is_same< FailCodeType, decltype(customAction_(e)) >::value>();
            return handleNonVoidExceptionType(shouldReturnActionResult);
            e; // prevent unused variable warning. 'e' is used inside the decltype, but nowhere else (except here...)
         }
      }

      FailCodeType handleException_(Bool2Type<true> exceptionTypeIsVoid)
      {
         // Handle the case of ExceptionType == void: Interpret this as catch-all: '...'
         try
         {
            return nestedHandler();
         }
         catch (...)
         {
            // If the return type of the custom action is the same as FailCodeType (as opposed to void or even less plausible something else)
            // then return the result of custom action as the fail code.
            auto shouldReturnActionResult = Bool2Type< std::is_same< FailCodeType, decltype(customAction_()) >::value>();
            return handleVoidExceptionType(shouldReturnActionResult);
         }
      }

      FailCodeType handleVoidExceptionType(Bool2Type<true> shouldReturnFromCustomAction)
      {
         try { return customAction_(); } catch (...) {} // Run custom action. Force it no-throw by suppressing any exceptions
         return failCode_;
      }

      FailCodeType handleVoidExceptionType(Bool2Type<false> shouldReturnFromCustomAction)
      {
         try { customAction_(); } catch (...) {} // Run custom action. Force it no-throw by suppressing any exceptions
         return failCode_;
      }

      FailCodeType handleNonVoidExceptionType(Bool2Type<true> shouldReturnFromCustomAction)
      {
         try 
         { 
            try 
            { throw; } // rethrow to get 'e' - can't pass it as argument since it may be of type void
            catch (ExceptionType& e)
            {
               // now we have 'e':
               try { return customAction_(e); } catch (...) {} // Run custom action. Force it no-throw by suppressing any exceptions
               return failCode_;
            }
         } catch (...) {} // In case from some weird situation the re-thrown exception is not of type ExceptionType
         return failCode_;
      }

      FailCodeType handleNonVoidExceptionType(Bool2Type<false> shouldReturnFromCustomAction)
      {
         try 
         { 
            try 
            { throw; } // rethrow to get 'e' - can't pass it as argument since it may be of type void
            catch (ExceptionType& e)
            {
               // now we have 'e':
               try { customAction_(e); } catch (...) {} // Run custom action. Force it no-throw by suppressing any exceptions
               return failCode_;
            }
         } catch (...) {} // In case from some weird situation the re-thrown exception is not of type ExceptionType
         return failCode_;
      }

   private:
      FailCodeType failCode_;
      NestedHandler nestedHandler;
      Callable customAction_; 
   };


   template <typename ExceptionType, typename Callable, typename NestedHandler>
   inline GenericExceptionHandler<ExceptionType, NestedHandler, Callable> generic_handler(typename NestedHandler::FailCodeType failCode, Callable customAction, NestedHandler const& nestedHandler)
   {  return GenericExceptionHandler<ExceptionType, NestedHandler, Callable>(failCode, nestedHandler, customAction); }

   template <typename ExceptionType, typename NestedHandler>
   inline GenericExceptionHandler<ExceptionType, NestedHandler, DefaultNoAction<ExceptionType>> generic_handler(typename NestedHandler::FailCodeType failCode, NestedHandler const& nestedHandler)
   {  return generic_handler<ExceptionType>(failCode, DefaultNoAction<ExceptionType>(), nestedHandler); }

   template <typename ExceptionType, typename Callable, typename FailCodeType>
   inline GenericExceptionHandler<ExceptionType, UnHandler<FailCodeType>, Callable> generic_handler(FailCodeType failCode, Callable customAction)
   {  return generic_handler<ExceptionType>(failCode, customAction, UnHandler<FailCodeType>()); }

   template <typename ExceptionType, typename FailCodeType>
   inline GenericExceptionHandler<ExceptionType, UnHandler<FailCodeType>, DefaultNoAction<ExceptionType>> generic_handler(FailCodeType failCode)
   {  return generic_handler<ExceptionType>(failCode, DefaultNoAction<ExceptionType>(), UnHandler<FailCodeType>()); }
}
