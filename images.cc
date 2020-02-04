#include "images.h"
#include "functional"

template<>
Base_image<Fraction> constant(Fraction f) {
  assert(0 <= f && f <= 1);
  return [f](const Point p) {
    (void)p;
    return f;
  };
}

Image cond(Region region, Image this_way, Image that_way) {
  return [=](const Point p) {
    if (region(p))
      return this_way(p);
    return that_way(p);
  };
}

Image lerp(Blend blend, Image this_way, Image that_way) {
  return [=](const Point p) {
    return this_way(p).weighted_mean(that_way(p), blend(p));
  };
}

Image darken(Image image, Blend blend) {
    return lerp(blend, image, constant(Colors::black));
}

Image lighten(Image image, Blend blend) {
    return lerp(blend, image, constant(Colors::white));
}
