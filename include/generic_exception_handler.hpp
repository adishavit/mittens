#pragma once

#include <iostream>
#include <type_traits> // for std::is_same<>
#include <functional>
#include <type_traits> // for std::false_type/std::true_type
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
         // Instead of a call to `throw` we will check if the function was called inside a
         // catch clause. If so, `std::rethrow_exception(eptr)` is the same as `throw`.
         // Otherwise, throw an `std::logic_error` since this is obviously a bug.
         if (std::exception_ptr eptr = std::current_exception())
            std::rethrow_exception(eptr);

         throw std::logic_error("Mittens handler called outside of a catch clause! This is a BUG.");
      }
      FailCodeType operator()() { return handleException(); }
   };


   //////////////////////////////////////////////////////////////////////////

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

      GenericExceptionHandler(FailCodeType failCode, NestedHandler const& nestedHandler, Callable customAction = Callable(), bool supressExceptionsInAction = true):
         failCode_(failCode),
         nestedHandler(nestedHandler),
         customAction_(customAction),
         supressExceptionsInAction_(supressExceptionsInAction)
      {}

      FailCodeType operator()() { return handleException(); }
      FailCodeType handleException() 
      {
         // Handle the general non-catch-all case 
         static_assert(!std::is_same<void, ExceptionType>::value, "Something wrong: specialization failed. ExceptionType is 'void'");
         try
         {
            return nestedHandler();
         }
         catch (ExceptionType& e)
         {
            try
            {
               // Run custom action, passing it 'e'
               // If the return type of the custom action is the same as FailCodeType (as opposed to void or even less plausible something else)
               // then return the result of custom action as the fail code.
               using ReturnActionResult = std::is_same< FailCodeType, decltype(customAction_(e))>::type;
               return runAction<ReturnActionResult>(e);
            }
            catch (...)
            {
               // Force the action no-throw by suppressing any exceptions unless requested not to.
               if (!supressExceptionsInAction_)
                  throw;
            }
            return failCode_;
         }
      }

   private:
      template <typename = std::true_type>
      FailCodeType runAction(ExceptionType& e)
      {
         return customAction_(e);
      }

      template <>
      FailCodeType runAction<std::false_type>(ExceptionType& e)
      {
         customAction_(e);
         return failCode_;
      }


      //////////////////////////////////////////////////////////////////////////

   private:
      FailCodeType failCode_;
      NestedHandler nestedHandler;
      Callable customAction_; 
      bool supressExceptionsInAction_;
   };

   //////////////////////////////////////////////////////////////////////////
   // Specialize for the catch-all (...) case. Indicated by ExceptionType being void
   template <typename NestedHandler, typename Callable>
   class GenericExceptionHandler<void, NestedHandler, Callable>
   {
   public:
      typedef typename NestedHandler::FailCodeType FailCodeType;

      GenericExceptionHandler(FailCodeType failCode, NestedHandler const& nestedHandler, Callable customAction = Callable(), bool supressExceptionsInAction = true) :
         failCode_(failCode),
         nestedHandler(nestedHandler),
         customAction_(customAction),
         supressExceptionsInAction_(supressExceptionsInAction)
      {}

      FailCodeType operator()() { return handleException(); }
      FailCodeType handleException()
      {
         try
         {
            return nestedHandler();
         }
         catch (...)
         {            
            try 
            { 
               // Run custom action. 
               // If the return type of the custom action is the same as FailCodeType (as opposed to void or even less plausible something else)
               // then return the result of custom action as the fail code.
               using ReturnActionResult = std::is_same< FailCodeType, decltype(customAction_())>::type;
               return runAction<ReturnActionResult>(); 
            }
            catch (...) 
            { 
               // Force the action no-throw by suppressing any exceptions unless requested not to.
               if (!supressExceptionsInAction_)
                  throw; 
            } 
            return failCode_;
         }
      }

   private:

      template <typename = std::true_type>
      FailCodeType runAction() 
      { 
         return customAction_(); 
      }

      template <>
      FailCodeType runAction<std::false_type>() 
      {
         customAction_();
         return failCode_;
      }

   private:
      FailCodeType failCode_;
      NestedHandler nestedHandler;
      Callable customAction_;
      bool supressExceptionsInAction_;
   };


   template <typename ExceptionType, typename Callable, typename NestedHandler>
   inline auto generic_handler(typename NestedHandler::FailCodeType failCode, Callable customAction, NestedHandler const& nestedHandler, bool supressExceptionsInAction = true)
   {  return GenericExceptionHandler<ExceptionType, NestedHandler, Callable>(failCode, nestedHandler, customAction, supressExceptionsInAction); }

   namespace
   {
      namespace detail
      {
         //////////////////////////////////////////////////////////////////////////
         // Helper functions for SFINAE overload resolution
         template< typename ... Ts >
         using void_t = void;

         template< typename T, typename = void >
         struct hasNestedFailCodeType : std::false_type {};

         template< typename T >
         struct hasNestedFailCodeType< T, void_t<typename T::FailCodeType> > : std::true_type {};
         //////////////////////////////////////////////////////////////////////////

         template <typename ExceptionType, typename Callable, typename FailCodeType>
         inline auto generic_handler_impl(FailCodeType failCode, Callable customAction, bool supressExceptionsInAction, std::false_type)
         {  return generic_handler<ExceptionType>(failCode, customAction, UnHandler<FailCodeType>(), supressExceptionsInAction);          }

         template <typename ExceptionType, typename NestedHandler, typename FailCodeType>
         inline auto generic_handler_impl(FailCodeType failCode, NestedHandler const& nestedHandler, bool supressExceptionsInAction, std::true_type)
         {  return generic_handler<ExceptionType>(failCode, DefaultNoAction<ExceptionType>(), nestedHandler, supressExceptionsInAction);  }
      }
   }

   template <typename ExceptionType, typename Callable, typename FailCodeType>
   inline auto generic_handler(FailCodeType code, Callable func, bool supressExceptionsInAction = true)
   {  return detail::generic_handler_impl<ExceptionType>(code, func, supressExceptionsInAction, detail::hasNestedFailCodeType<Callable>{}); }

   template <typename ExceptionType, typename FailCodeType>
   inline auto generic_handler(FailCodeType failCode)
   {  return generic_handler<ExceptionType>(failCode, DefaultNoAction<ExceptionType>(), UnHandler<FailCodeType>()); }
}
