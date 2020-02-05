#include "images.h"

Image cond(Region region, Image this_way, Image that_way) {
  return lift(
    [](bool b, Color c1, Color c2) {
      return b ? c1 : c2;
    },
    region,
    this_way,
    that_way
  );
}

Image lerp(Blend blend, Image this_way, Image that_way) {
  return lift(
    [](Fraction f, Color c1, Color c2) {
      return c1.weighted_mean(c2, f);
    },
    blend,
    this_way,
    that_way
  );
}

Image darken(Image image, Blend blend) {
    return lerp(blend, image, constant(Colors::black));
}

Image lighten(Image image, Blend blend) {
    return lerp(blend, image, constant(Colors::white));
}
