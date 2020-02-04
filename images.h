#ifndef JNP_IMAGES_H
#define JNP_IMAGES_H

#include <functional>
#include <cassert>
#include "coordinate.h"
#include "color.h"

using namespace std;

using Fraction = float;

template<typename T>
using Base_image = function<T(const Point)>;

using Region = Base_image<bool>;

using Image = Base_image<Color>;

using Blend = Base_image<Fraction>;

template<typename T>
Base_image<T> constant(T t) {
  return [t](const Point p) {
    return t;
    (void)p;
  };
}

// template<>
// Base_image<Fraction> constant(Fraction f) {
//   assert(0 <= f && f <= 1);
//   return [f](const Point p) {
//     (void)p;
//     return f;
//   };
// }

template<typename T>
Base_image<T> rotate(Base_image<T> image, double phi) {
  return [phi, image](const Point p) {
    Point np = p.is_polar ? p : to_polar(p);
    return image(Point(np.first, np.second + phi, np.is_polar));
  };
}

template<typename T>
Base_image<T> translate(Base_image<T> image, Vector v) {
  return [image, v](const Point p) {
    Point np = p.is_polar ? from_polar(p) : p;
    return image(Point(np.first + v.first, np.second + v.second, np.is_polar));
  };
}

template<typename T>
Base_image<T> scale(Base_image<T> image, double s) {
  return [image, s](const Point p) {
    Point np = p.is_polar ? p : to_polar(p);
    return image(Point(np.first / s, np.second, np.is_polar));
  };
}

template<typename T>
Base_image<T> circle(Point q, double r, T inner, T outer) {
  return [q, r, inner, outer](const Point p) {
    Point np = p.is_polar ? from_polar(p) : p;
    if (distance(np, q) <= r) {
      return inner;
    }
    return outer;
  };
}

// Base_image<T> checker(double d, T this_way, T that_way);

// Base_image<T> polar_checker(double d, int n, T this_way, T that_way);

// Base_image<T> rings(Point q, double d, T this_way, T that_way);

template <typename T>
Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
  return [d, this_way, that_way](const Point p) {
    Point np = p.is_polar ? from_polar(p) : p;
    if (abs(np.first)*2 <= d) {
      return this_way;
    }
    return that_way;
  };
}

// Image cond(Region region, Image this_way, Image that_way);

// Image lerp(Blend blend, Image this_way, Image that_way);

// Image darken(Image image, Blend blend);

// Image lighten(Image image, Blend blend);


#endif /* JNP_IMAGES_H */