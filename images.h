#ifndef JNP_IMAGES_H
#define JNP_IMAGES_H

#include <functional>
#include <cassert>
#include "functional.h"
#include "coordinate.h"
#include "color.h"

using namespace std;

template<typename T>
using Base_image = function<T(const Point)>;

using Fraction = double;
using Region = Base_image<bool>;
using Image = Base_image<Color>;
using Blend = Base_image<Fraction>;

template<typename T>
Base_image<T> constant(T t) {
  return compose(
    [=](const Point p) {
      return t;
      (void)p;
    }
  );
}

template<typename T>
Base_image<T> rotate(Base_image<T> image, double phi) {
  return compose(
    to_polar,
    [=](const Point p) {
      return image(Point(p.first, p.second - phi, true));
    }
  );
}

template<typename T>
Base_image<T> translate(Base_image<T> image, Vector v) {
  return compose(
    [=](const Point p) {
      return Point(p.first + v.first, p.second + v.second);
    },
    image
  );
}

template<typename T>
Base_image<T> scale(Base_image<T> image, double s) {
  return compose(
    to_polar,
    [s](const Point p) {
      return Point(p.first / s, p.second, true);
    },
    from_polar,
    image
  );
}

template<typename T>
Base_image<T> circle(Point q, double r, T inner, T outer) {
  // return lift(
  //   [](auto b, auto c1, auto c2) {
  //     return b ? c1 : c2;
  //   },
  //   [q, r](const Point p) {
  //     return distance(p, q) <= r;
  //   },
  //   inner,
  //   outer
  // );
  return compose(
    [=](const Point p) {
      return distance(p, q) <= r ? inner : outer;
    }
  );
}

template<typename T>
Base_image<T> checker(double d, T this_way, T that_way) {
  return compose(
    [=](const Point p) {
      return static_cast<int>(floor(p.first / d) + floor(p.second / d)) % 2 ? that_way : this_way;
    }
  );
}

template<typename T>
Base_image<T> polar_checker(double d, int n, T this_way, T that_way) {
  assert(n);
  return compose(
    to_polar,
    [=](const Point p) {
      return Point(p.first, p.second * static_cast<double>(n) * d /(2 * M_PI), true);
    },
    checker(d, this_way, that_way)
  );
}

template <typename T>
Base_image<T> rings(Point q, double d, T this_way, T that_way) {
  return compose(
    [=](const Point p) {
      return static_cast<int>(distance(p, q) / d) % 2 ? that_way : this_way;
    }
  );
}

template <typename T>
Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
  return compose(
    [=](const Point p) {
      return abs(p.first) * 2 <= d ? this_way : that_way;
    }
  );
}

Image cond(Region region, Image this_way, Image that_way);

Image lerp(Blend blend, Image this_way, Image that_way);

Image darken(Image image, Blend blend);

Image lighten(Image image, Blend blend);


#endif /* JNP_IMAGES_H */