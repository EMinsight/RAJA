#ifndef NEW_REDUCE_TBB_REDUCE_HPP
#define NEW_REDUCE_TBB_REDUCE_HPP



#if defined(RAJA_ENABLE_TBB)
namespace detail {

  // Init
  template<typename EXEC_POL, typename OP, typename T>
  camp::concepts::enable_if< std::is_same< EXEC_POL, RAJA::tbb_for_dynamic> >
  init(Reducer<OP, T>& red) {
    red.val = Reducer<OP,T>::op::identity();
  }
  // Combine
  template<typename EXEC_POL, typename OP, typename T>
  camp::concepts::enable_if< std::is_same< EXEC_POL, RAJA::tbb_for_dynamic> >
  combine(Reducer<OP, T>& out, const Reducer<OP, T>& in) {
    out.val = typename Reducer<OP,T>::op{}(out.val, in.val);
  }
  // Resolve
  template<typename EXEC_POL, typename OP, typename T>
  camp::concepts::enable_if< std::is_same< EXEC_POL, RAJA::tbb_for_dynamic> >
  resolve(Reducer<OP, T>& red) {
    *red.target = red.val;
  }

} //  namespace detail
#endif

#endif //  NEW_REDUCE_SEQ_REDUCE_HPP
