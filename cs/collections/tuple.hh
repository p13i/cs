#ifndef CS_COLLECTIONS_TUPLE_HH
#define CS_COLLECTIONS_TUPLE_HH

namespace cs::collections {

template <class T1, class T2>
struct Tuple {
  T1 first_;
  T2 second_;
  Tuple(T1 first, T2 second)
      : first_(first), second_(second) {}
  T1 first() { return first_; }
  T2 second() { return second_; }

  friend std::ostream& operator<<(std::ostream& os,
                                  const Tuple& tuple) {
    return os << "<" << tuple.first_ << ", "
              << tuple.second_ << ">";
  }
};

}  // namespace cs::collections

#endif  // CS_COLLECTIONS_TUPLE_HH