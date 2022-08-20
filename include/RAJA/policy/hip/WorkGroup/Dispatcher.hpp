/*!
 ******************************************************************************
 *
 * \file
 *
 * \brief   Header file containing RAJA workgroup Dispatcher.
 *
 ******************************************************************************
 */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-22, Lawrence Livermore National Security, LLC
// and RAJA project contributors. See the RAJA/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJA_hip_WorkGroup_Dispatcher_HPP
#define RAJA_hip_WorkGroup_Dispatcher_HPP

#include "RAJA/config.hpp"

#include "RAJA/policy/hip/policy.hpp"

#include "RAJA/pattern/WorkGroup/Dispatcher.hpp"

#include <thread>
#include <mutex>


namespace RAJA
{

namespace detail
{

namespace hip
{

// global function that gets the device function pointer and
// writes it into a pinned ptr
template < typename invoker_type, typename InvokerGetter >
__global__ void get_invoker_global(
    invoker_type* ptr, InvokerGetter invokerGetter)
{
  *ptr = invokerGetter();
}

// get the pinned ptr buffer
inline void* get_cached_invoker_ptr(size_t nbytes)
{
  static size_t cached_nbytes = 0;
  static void* ptr = nullptr;
  if (nbytes > cached_nbytes) {
    cached_nbytes = 0;
    hipErrchk(hipHostFree(ptr));
    hipErrchk(hipHostMalloc(&ptr, nbytes));
    cached_nbytes = nbytes;
  }
  return ptr;
}

// mutex that guards against concurrent use of
// pinned buffer and get_cached_invoker_ptr()
inline std::mutex& get_invoker_mutex()
{
  static std::mutex s_mutex;
  return s_mutex;
}

// get the device function pointer by calling a global function to
// write it into a pinned ptr, beware different instantiates of this
// function may run concurrently
template < typename invoker_type, typename InvokerGetter >
inline auto get_invoker(InvokerGetter&& invokerGetter)
{
  const std::lock_guard<std::mutex> lock(get_invoker_mutex());

  auto ptr = static_cast<invoker_type*>(get_cached_invoker_ptr(sizeof(invoker_type)));
  auto func = get_invoker_global<invoker_type, std::decay_t<InvokerGetter>>;
  hipLaunchKernelGGL(func, dim3(1), dim3(1), 0, 0,
                     ptr, std::forward<InvokerGetter>(invokerGetter));
  hipErrchk(hipGetLastError());
  hipErrchk(hipDeviceSynchronize());

  return *ptr;
}

template < typename invoker_type, typename InvokerGetter >
inline auto get_cached_invoker(InvokerGetter&& invokerGetter)
{
  static auto invoker = get_invoker<invoker_type>(std::forward<InvokerGetter>(invokerGetter));
  return invoker;
}

}  // namespace hip

/*!
* Populate and return a Dispatcher object where the
* call operator is a device function
*/
template < typename T, typename Dispatcher_T, size_t BLOCK_SIZE, bool Async >
inline const Dispatcher_T* get_Dispatcher(hip_work<BLOCK_SIZE, Async> const&)
{
  using invoker_type = typename Dispatcher_T::invoker_type;
  static Dispatcher_T dispatcher{
        Dispatcher_T::template makeDeviceDispatcher<T>(
          [](auto&& invokerGetter) {
            return hip::get_cached_invoker<invoker_type>(
                std::forward<decltype(invokerGetter)>(invokerGetter));
          }) };
  return &dispatcher;
}

}  // namespace detail

}  // namespace RAJA

#endif  // closing endif for header file include guard
