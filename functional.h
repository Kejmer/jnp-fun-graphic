#ifndef JNP_FUNCTIONAL_H
#define JNP_FUNCTIONAL_H


template <typename F, typename... Args>
auto compose(F f, Args... args) {
  return [=](auto x) {
    return compose(args...)(f(x));
  };
}

auto compose() {
  return [](auto x) {
    return x;
  };
}

template <typename H, typename... Args>
auto lift(H h,Args... args) {
  return [=](auto a) {
    return h(args(a)...);
  };
}


#endif