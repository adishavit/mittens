#pragma once

#include <iostream>

#include "generic_exception_handler.hpp"

namespace mittens
{
   //////////////////////////////////////////////////////////////////////////
   // Catch-all handler maker
   template <typename NestedHandler, typename Callable>
   inline auto all_catcher(typename NestedHandler::FailCodeType failCode, Callable customAction, NestedHandler const& nestedHandler, bool supressExceptionsInAction = true)
   {  return generic_handler<void>(failCode, customAction, nestedHandler, supressExceptionsInAction); }

   template <typename Callable, typename FailCodeType>
   inline auto all_catcher(FailCodeType failCode, Callable func, bool supressExceptionsInAction = true)
   {  return generic_handler<void>(failCode, func, supressExceptionsInAction); }

   template <typename FailCodeType>
   inline auto all_catcher(FailCodeType failCode)
   {  return generic_handler<void>(failCode); }

   //////////////////////////////////////////////////////////////////////////
   // std::exception handler maker
   template <typename NestedHandler, typename Callable>
   inline auto std_exception_handler(typename NestedHandler::FailCodeType failCode, Callable customAction, NestedHandler const& nestedHandler, bool supressExceptionsInAction = true)
   {  return generic_handler<std::exception>(failCode, customAction, nestedHandler, supressExceptionsInAction); }

   template <typename Callable, typename FailCodeType>
   inline auto std_exception_handler(FailCodeType failCode, Callable func, bool supressExceptionsInAction = true)
   {  return generic_handler<std::exception>(failCode, func, supressExceptionsInAction); }

   template <typename FailCodeType>
   inline auto std_exception_handler(FailCodeType failCode)
   {  return generic_handler<std::exception>(failCode); }

   //////////////////////////////////////////////////////////////////////////
   // std::bad_alloc handler maker
   template <typename NestedHandler, typename Callable>
   inline auto bad_alloc_handler(typename NestedHandler::FailCodeType failCode, Callable customAction, NestedHandler const& nestedHandler, bool supressExceptionsInAction = true)
   {  return generic_handler<std::bad_alloc>(failCode, customAction, nestedHandler, supressExceptionsInAction); }

   template <typename Callable, typename FailCodeType>
   inline auto bad_alloc_handler(FailCodeType failCode, Callable func, bool supressExceptionsInAction = true)
   {  return generic_handler<std::bad_alloc>(failCode, func, supressExceptionsInAction); }

   template <typename FailCodeType>
   inline auto bad_alloc_handler(FailCodeType failCode)
   {  return generic_handler<std::bad_alloc>(failCode); }
}

