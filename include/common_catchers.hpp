#pragma once

#include <iostream>

#include "generic_exception_catcher.hpp"

namespace mittens
{
   //////////////////////////////////////////////////////////////////////////
   // Catch-all handler maker
   template <typename NestedHandler>
   inline GenericExceptionCatcher<void, NestedHandler> all_catcher(typename NestedHandler::FailCodeType failCode, NestedHandler const& nestedHandler)
   {  return GenericExceptionCatcher<void, NestedHandler>(failCode, nestedHandler); }

   template <typename FailCodeType>
   inline GenericExceptionCatcher<void, UnCatcher<FailCodeType>> all_catcher(FailCodeType failCode)
   {  return GenericExceptionCatcher<void, UnCatcher<FailCodeType>>(failCode, UnCatcher<FailCodeType>()); }

   //////////////////////////////////////////////////////////////////////////
   // std::exception handler maker
   template <typename NestedHandler>
   inline GenericExceptionCatcher<std::exception, NestedHandler> std_exception_catcher(typename NestedHandler::FailCodeType failCode, NestedHandler const& nestedHandler)
   {  return GenericExceptionCatcher<std::exception, NestedHandler>(failCode, nestedHandler); }

   template <typename FailCodeType>
   inline GenericExceptionCatcher<std::exception, UnCatcher<FailCodeType>> std_exception_catcher(FailCodeType failCode)
   {  return GenericExceptionCatcher<std::exception, UnCatcher<FailCodeType>>(failCode, UnCatcher<FailCodeType>()); }

   //////////////////////////////////////////////////////////////////////////
   // std::bad_alloc handler maker
   template <typename NestedHandler>
   inline GenericExceptionCatcher<std::bad_alloc, NestedHandler> bad_alloc_catcher(typename NestedHandler::FailCodeType failCode, NestedHandler const& nestedHandler)
   {  return GenericExceptionCatcher<std::bad_alloc, NestedHandler>(failCode, nestedHandler); }

   template <typename FailCodeType>
   inline GenericExceptionCatcher<std::bad_alloc, UnCatcher<FailCodeType>> bad_alloc_catcher(FailCodeType failCode)
   {  return GenericExceptionCatcher<std::bad_alloc, UnCatcher<FailCodeType>>(failCode, UnCatcher<FailCodeType>()); }



}

