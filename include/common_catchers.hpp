#pragma once

#include <iostream>

#include "generic_exception_handler.hpp"

namespace mittens
{
   //////////////////////////////////////////////////////////////////////////
   // Catch-all handler maker
   template <typename NestedHandler>
   inline GenericExceptionHandler<void, NestedHandler> all_catcher(typename NestedHandler::FailCodeType failCode, NestedHandler const& nestedHandler)
   {  return GenericExceptionHandler<void, NestedHandler>(failCode, nestedHandler); }

   template <typename FailCodeType>
   inline GenericExceptionHandler<void, UnHandler<FailCodeType>> all_catcher(FailCodeType failCode)
   {  return GenericExceptionHandler<void, UnHandler<FailCodeType>>(failCode, UnHandler<FailCodeType>()); }

   //////////////////////////////////////////////////////////////////////////
   // std::exception handler maker
   template <typename NestedHandler>
   inline GenericExceptionHandler<std::exception, NestedHandler> std_exception_catcher(typename NestedHandler::FailCodeType failCode, NestedHandler const& nestedHandler)
   {  return GenericExceptionHandler<std::exception, NestedHandler>(failCode, nestedHandler); }

   template <typename FailCodeType>
   inline GenericExceptionHandler<std::exception, UnHandler<FailCodeType>> std_exception_catcher(FailCodeType failCode)
   {  return GenericExceptionHandler<std::exception, UnHandler<FailCodeType>>(failCode, UnHandler<FailCodeType>()); }

   //////////////////////////////////////////////////////////////////////////
   // std::bad_alloc handler maker
   template <typename NestedHandler>
   inline GenericExceptionHandler<std::bad_alloc, NestedHandler> bad_alloc_catcher(typename NestedHandler::FailCodeType failCode, NestedHandler const& nestedHandler)
   {  return GenericExceptionHandler<std::bad_alloc, NestedHandler>(failCode, nestedHandler); }

   template <typename FailCodeType>
   inline GenericExceptionHandler<std::bad_alloc, UnHandler<FailCodeType>> bad_alloc_catcher(FailCodeType failCode)
   {  return GenericExceptionHandler<std::bad_alloc, UnHandler<FailCodeType>>(failCode, UnHandler<FailCodeType>()); }
}

