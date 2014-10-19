#pragma once

#include <iostream>

#include "generic_exception_handler.hpp"

namespace mittens
{
   //////////////////////////////////////////////////////////////////////////
   // Catch-all handler maker
   template <typename NestedHandler, typename Callable>
   inline GenericExceptionHandler<void, NestedHandler, Callable> all_catcher(typename NestedHandler::FailCodeType failCode, Callable customAction, NestedHandler const& nestedHandler, bool supressExceptionsInAction = true)
   {  return generic_handler<void>(failCode, customAction, nestedHandler, supressExceptionsInAction); }

   template <typename NestedHandler>
   inline GenericExceptionHandler<void, NestedHandler> all_catcher(typename NestedHandler::FailCodeType failCode, NestedHandler const& nestedHandler, bool supressExceptionsInAction = true)
   {  return generic_handler<void>(failCode, nestedHandler, supressExceptionsInAction); }

   template <typename Callable, typename FailCodeType>
   inline GenericExceptionHandler<void, UnHandler<FailCodeType>, Callable> all_catcher(FailCodeType failCode, Callable customAction, bool supressExceptionsInAction = true)
   {  return generic_handler<void>(failCode, customAction, supressExceptionsInAction); }

   template <typename FailCodeType>
   inline GenericExceptionHandler<void, UnHandler<FailCodeType>> all_catcher(FailCodeType failCode, bool supressExceptionsInAction = true)
   {  return generic_handler<void>(failCode, supressExceptionsInAction); }

   //////////////////////////////////////////////////////////////////////////
   // std::exception handler maker
   template <typename NestedHandler, typename Callable>
   inline GenericExceptionHandler<std::exception, NestedHandler, Callable> std_exception_handler(typename NestedHandler::FailCodeType failCode, Callable customAction, NestedHandler const& nestedHandler, bool supressExceptionsInAction = true)
   {  return generic_handler<std::exception>(failCode, customAction, nestedHandler, supressExceptionsInAction); }

   template <typename NestedHandler>
   inline GenericExceptionHandler<std::exception, NestedHandler> std_exception_handler(typename NestedHandler::FailCodeType failCode, NestedHandler const& nestedHandler, bool supressExceptionsInAction = true)
   {  return generic_handler<std::exception>(failCode, nestedHandler, supressExceptionsInAction); }

   template <typename Callable, typename FailCodeType>
   inline GenericExceptionHandler<std::exception, UnHandler<FailCodeType>, Callable> std_exception_handler(FailCodeType failCode, Callable customAction, bool supressExceptionsInAction = true)
   {  return generic_handler<std::exception>(failCode, customAction, supressExceptionsInAction); }

   template <typename FailCodeType>
   inline GenericExceptionHandler<std::exception, UnHandler<FailCodeType>> std_exception_handler(FailCodeType failCode, bool supressExceptionsInAction = true)
   {  return generic_handler<std::exception>(failCode, supressExceptionsInAction); }

   //////////////////////////////////////////////////////////////////////////
   // std::bad_alloc handler maker
   template <typename NestedHandler, typename Callable>
   inline GenericExceptionHandler<std::bad_alloc, NestedHandler, Callable> bad_alloc_handler(typename NestedHandler::FailCodeType failCode, Callable customAction, NestedHandler const& nestedHandler, bool supressExceptionsInAction = true)
   {  return generic_handler<std::bad_alloc>(failCode, customAction, nestedHandler, supressExceptionsInAction); }

   template <typename NestedHandler>
   inline GenericExceptionHandler<std::bad_alloc, NestedHandler> bad_alloc_handler(typename NestedHandler::FailCodeType failCode, NestedHandler const& nestedHandler, bool supressExceptionsInAction = true)
   {  return generic_handler<std::bad_alloc>(failCode, nestedHandler, supressExceptionsInAction); }

   template <typename Callable, typename FailCodeType>
   inline GenericExceptionHandler<std::bad_alloc, UnHandler<FailCodeType>, Callable> bad_alloc_handler(FailCodeType failCode, Callable customAction, bool supressExceptionsInAction = true)
   {  return generic_handler<std::bad_alloc>(failCode, customAction, supressExceptionsInAction); }

   template <typename FailCodeType>
   inline GenericExceptionHandler<std::bad_alloc, UnHandler<FailCodeType>> bad_alloc_handler(FailCodeType failCode, bool supressExceptionsInAction = true)
   {  return generic_handler<std::bad_alloc>(failCode, supressExceptionsInAction); }
}

