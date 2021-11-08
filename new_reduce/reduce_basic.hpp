#ifndef PROTO_NEW_REDUCE_HPP
#define PROTO_NEW_REDUCE_HPP

#include "util/valloc.hpp"

#if defined(RAJA_ENABLE_CUDA)
#define DEVICE cuda
#elif defined(RAJA_ENABLE_HIP)
#define DEVICE hip
#endif

namespace detail
{

  //
  //
  // Basic Reducer
  //
  //
  template <typename Op, typename T>
  struct Reducer {
    using op = Op;
    using val_type = T;
    RAJA_HOST_DEVICE Reducer() {}
    Reducer(T *target_in) : target(target_in), val(op::identity()) {}
    T *target = nullptr;
    T val = op::identity();

#if defined(RAJA_ENABLE_CUDA) || defined(RAJA_ENABLE_HIP)
    // Device related attributes.
    T * devicetarget = nullptr;
    RAJA::detail::SoAPtr<T, RAJA::DEVICE::device_mempool_type> device_mem;
    unsigned int * device_count = nullptr;
#elif defined(RAJA_ENABLE_SYCL)
    camp::resources::Resource * sycl_res = nullptr;
#endif

    static constexpr size_t num_lambda_args = 1;
    RAJA_HOST_DEVICE auto get_lambda_arg_tup() { return camp::make_tuple(&val); }
  };

} // namespace detail

#include "sequential/reduce.hpp"
#include "openmp/reduce.hpp"
#include "omp-target/reduce.hpp"
#include "cuda/reduce.hpp"
#include "hip/reduce.hpp"
#include "sycl/reduce.hpp"

template <template <typename, typename, typename> class Op, typename T>
auto constexpr Reduce(T *target)
{
  return detail::Reducer<Op<T, T, T>, T>(target);
}

template <typename T>
auto constexpr ReduceLoc(ValLocMin<T> *target) 
{
  using R = ValLocMin<T>;
  return detail::Reducer<RAJA::operators::minimum<R,R,R>, R>(target);
}
template <typename T>
auto constexpr ReduceLoc(ValLocMax<T> *target)
{
  using R = ValLocMax<T>;
  return detail::Reducer<RAJA::operators::maximum<R,R,R>, R>(target);
}

#endif //  PROTO_NEW_REDUCE_HPP
