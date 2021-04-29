#ifndef NEW_REDUCE_TBB_REDUCE_HPP
#define NEW_REDUCE_TBB_REDUCE_HPP



namespace detail {

  // Init
  template<typename EXEC_POL, typename OP, typename T>
  camp::concepts::enable_if_t< bool, std::is_same< EXEC_POL, RAJA::tbb_for_dynamic> > // Returning bool because void param loop machine broken.
  init(Reducer<OP, T>& red) {
    red.val = Reducer<OP,T>::op::identity();
    return true;
  }
  // Combine
  template<typename EXEC_POL, typename OP, typename T>
  camp::concepts::enable_if_t< bool, std::is_same< EXEC_POL, RAJA::tbb_for_dynamic> > // Returning bool because void param loop machine broken.
  combine(Reducer<OP, T>& out, const Reducer<OP, T>& in) {
    out.val = typename Reducer<OP,T>::op{}(out.val, in.val);
    return true;
  }
  // Resolve
  template<typename EXEC_POL, typename OP, typename T>
  camp::concepts::enable_if_t< bool, std::is_same< EXEC_POL, RAJA::tbb_for_dynamic> > // Returning bool because void param loop machine broken.
  resolve(Reducer<OP, T>& red) {
    *red.target = red.val;
    return true;
  }

} //  namespace detail

#endif //  NEW_REDUCE_SEQ_REDUCE_HPP
