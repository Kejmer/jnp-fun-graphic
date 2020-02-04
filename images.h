#ifndef JNP_IMAGES_H
#define JNP_IMAGES_H

#include <functional>
#include <cassert>
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
  return [=](const Point p) {
    return t;
    (void)p;
  };
}

template<typename T>
Base_image<T> rotate(Base_image<T> image, double phi) {
  return [=](const Point p) {
    Point np = p.is_polar ? p : to_polar(p);
    return image(Point(np.first, np.second - phi, np.is_polar));
  };
}

template<typename T>
Base_image<T> translate(Base_image<T> image, Vector v) {
  return [=](const Point p) {
    Point np = p.is_polar ? from_polar(p) : p;
    return image(Point(np.first + v.first, np.second + v.second, np.is_polar));
  };
}

template<typename T>
Base_image<T> scale(Base_image<T> image, double s) {
  return [=](const Point p) {
    Point np = p.is_polar ? p : to_polar(p);
    return image(Point(np.first / s, np.second, np.is_polar));
  };
}

template<typename T>
Base_image<T> circle(Point q, double r, T inner, T outer) {
  return [=](const Point p) {
    Point np = p.is_polar ? from_polar(p) : p;
    if (distance(np, q) <= r)
      return inner;
    return outer;
  };
}

template<typename T>
Base_image<T> checker(double d, T this_way, T that_way) {
  return [=](const Point p) {
    Point np = p.is_polar ? from_polar(p) : p;
    if (static_cast<int>(floor(np.first / d) + floor(np.second / d)) % 2)
      return that_way;
    return this_way;
  };
}

template<typename T>
Base_image<T> polar_checker(double d, int n, T this_way, T that_way) {
  assert(n);
  return [=](const Point p) {
    Point np = p.is_polar ? p : to_polar(p);
    return checker(d, this_way, that_way)(Point(np.first, np.second * static_cast<double>(n) * d /(2 * M_PI), !np.is_polar));
  };
}

template <typename T>
Base_image<T> rings(Point q, double d, T this_way, T that_way) {
  return [=](const Point p) {
    Point np = p.is_polar ? from_polar(p) : p;
    if (static_cast<int>(distance(np, q) / d) % 2)
      return that_way;
    return this_way;
  };
}

template <typename T>
Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
  return [=](const Point p) {
    Point np = p.is_polar ? from_polar(p) : p;
    if (abs(np.first)*2 <= d)
      return this_way;
    return that_way;
  };
}

Image cond(Region region, Image this_way, Image that_way);

Image lerp(Blend blend, Image this_way, Image that_way);

Image darken(Image image, Blend blend);

Image lighten(Image image, Blend blend);


#endif /* JNP_IMAGES_H */