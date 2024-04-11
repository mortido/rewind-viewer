#pragma once
#include <algorithm>
#include <cmath>

namespace rewind_viewer::colors {

#define EPS 1e-7

inline uint32_t heat_color(double value, double min_value, double max_value) {
  value = std::min(std::max(value, min_value), max_value - EPS);
  double d = max_value - min_value;
  value = d < EPS ? 0.5 : (value - min_value) / d;
  double m = 0.25;
  int range = std::floor(value / m);
  double s = (value - range * m) / m;
  double r, g, b;
  switch (range) {
    case 0 : r = 0.0;
      g = s;
      b = 1.0;
      break;
    case 1 : r = 0.0;
      g = 1.0;
      b = 1.0 - s;
      break;
    case 2 : r = s;
      g = 1.0;
      b = 0.0;
      break;
    case 3 : r = 1.0;
      g = 1.0 - s;
      b = 0.0;
      break;
  }

  auto R = static_cast<uint32_t>(255.0 * r);
  auto G = static_cast<uint32_t>(255.0 * g);
  auto B = static_cast<uint32_t>(255.0 * b);

  return R << 16 | G << 8 | B;
}

namespace red {
inline constexpr uint32_t IndianRed = 0xCD5C5C;
inline constexpr uint32_t LightCoral = 0xF08080;
inline constexpr uint32_t Salmon = 0xFA8072;
inline constexpr uint32_t DarkSalmon = 0xE9967A;
inline constexpr uint32_t LightSalmon = 0xFFA07A;
inline constexpr uint32_t Crimson = 0xDC143C;
inline constexpr uint32_t Red = 0xFF0000;
inline constexpr uint32_t FireBrick = 0xB22222;
inline constexpr uint32_t DarkRed = 0x8B0000;
}

namespace pink {
inline constexpr uint32_t Pink = 0xFFC0CB;
inline constexpr uint32_t LightPink = 0xFFB6C1;
inline constexpr uint32_t HotPink = 0xFF69B4;
inline constexpr uint32_t DeepPink = 0xFF1493;
inline constexpr uint32_t MediumVioletRed = 0xC71585;
inline constexpr uint32_t PaleVioletRed = 0xDB7093;
}

namespace orange {
inline constexpr uint32_t LightSalmon = 0xFFA07A;
inline constexpr uint32_t Coral = 0xFF7F50;
inline constexpr uint32_t Tomato = 0xFF6347;
inline constexpr uint32_t OrangeRed = 0xFF4500;
inline constexpr uint32_t DarkOrange = 0xFF8C00;
inline constexpr uint32_t Orange = 0xFFA500;
}

namespace yellow {
inline constexpr uint32_t Gold = 0xFFD700;
inline constexpr uint32_t Yellow = 0xFFFF00;
inline constexpr uint32_t LightYellow = 0xFFFFE0;
inline constexpr uint32_t LemonChiffon = 0xFFFACD;
inline constexpr uint32_t LightGoldenrodYellow = 0xFAFAD2;
inline constexpr uint32_t PapayaWhip = 0xFFEFD5;
inline constexpr uint32_t Moccasin = 0xFFE4B5;
inline constexpr uint32_t PeachPuff = 0xFFDAB9;
inline constexpr uint32_t PaleGoldenrod = 0xEEE8AA;
inline constexpr uint32_t Khaki = 0xF0E68C;
inline constexpr uint32_t DarkKhaki = 0xBDB76B;
}

namespace purple {
inline constexpr uint32_t Lavender = 0xE6E6FA;
inline constexpr uint32_t Thistle = 0xD8BFD8;
inline constexpr uint32_t Plum = 0xDDA0DD;
inline constexpr uint32_t Violet = 0xEE82EE;
inline constexpr uint32_t Orchid = 0xDA70D6;
inline constexpr uint32_t Fuchsia = 0xFF00FF;
inline constexpr uint32_t Magenta = 0xFF00FF;
inline constexpr uint32_t MediumOrchid = 0xBA55D3;
inline constexpr uint32_t MediumPurple = 0x9370DB;
inline constexpr uint32_t RebeccaPurple = 0x663399;
inline constexpr uint32_t BlueViolet = 0x8A2BE2;
inline constexpr uint32_t DarkViolet = 0x9400D3;
inline constexpr uint32_t DarkOrchid = 0x9932CC;
inline constexpr uint32_t DarkMagenta = 0x8B008B;
inline constexpr uint32_t Purple = 0x800080;
inline constexpr uint32_t Indigo = 0x4B0082;
inline constexpr uint32_t SlateBlue = 0x6A5ACD;
inline constexpr uint32_t DarkSlateBlue = 0x483D8B;
inline constexpr uint32_t MediumSlateBlue = 0x7B68EE;
}

namespace green {
inline constexpr uint32_t GreenYellow = 0xADFF2F;
inline constexpr uint32_t Chartreuse = 0x7FFF00;
inline constexpr uint32_t LawnGreen = 0x7CFC00;
inline constexpr uint32_t Lime = 0x00FF00;
inline constexpr uint32_t LimeGreen = 0x32CD32;
inline constexpr uint32_t PaleGreen = 0x98FB98;
inline constexpr uint32_t LightGreen = 0x90EE90;
inline constexpr uint32_t MediumSpringGreen = 0x00FA9A;
inline constexpr uint32_t SpringGreen = 0x00FF7F;
inline constexpr uint32_t MediumSeaGreen = 0x3CB371;
inline constexpr uint32_t SeaGreen = 0x2E8B57;
inline constexpr uint32_t ForestGreen = 0x228B22;
inline constexpr uint32_t Green = 0x008000;
inline constexpr uint32_t DarkGreen = 0x006400;
inline constexpr uint32_t YellowGreen = 0x9ACD32;
inline constexpr uint32_t OliveDrab = 0x6B8E23;
inline constexpr uint32_t Olive = 0x808000;
inline constexpr uint32_t DarkOliveGreen = 0x556B2F;
inline constexpr uint32_t MediumAquamarine = 0x66CDAA;
inline constexpr uint32_t DarkSeaGreen = 0x8FBC8B;
inline constexpr uint32_t LightSeaGreen = 0x20B2AA;
inline constexpr uint32_t DarkCyan = 0x008B8B;
inline constexpr uint32_t Teal = 0x008080;
}

namespace blue {
inline constexpr uint32_t Aqua = 0x00FFFF;
inline constexpr uint32_t Cyan = 0x00FFFF;
inline constexpr uint32_t LightCyan = 0xE0FFFF;
inline constexpr uint32_t PaleTurquoise = 0xAFEEEE;
inline constexpr uint32_t Aquamarine = 0x7FFFD4;
inline constexpr uint32_t Turquoise = 0x40E0D0;
inline constexpr uint32_t MediumTurquoise = 0x48D1CC;
inline constexpr uint32_t DarkTurquoise = 0x00CED1;
inline constexpr uint32_t CadetBlue = 0x5F9EA0;
inline constexpr uint32_t SteelBlue = 0x4682B4;
inline constexpr uint32_t LightSteelBlue = 0xB0C4DE;
inline constexpr uint32_t PowderBlue = 0xB0E0E6;
inline constexpr uint32_t LightBlue = 0xADD8E6;
inline constexpr uint32_t SkyBlue = 0x87CEEB;
inline constexpr uint32_t LightSkyBlue = 0x87CEFA;
inline constexpr uint32_t DeepSkyBlue = 0x00BFFF;
inline constexpr uint32_t DodgerBlue = 0x1E90FF;
inline constexpr uint32_t CornflowerBlue = 0x6495ED;
inline constexpr uint32_t MediumSlateBlue = 0x7B68EE;
inline constexpr uint32_t RoyalBlue = 0x4169E1;
inline constexpr uint32_t Blue = 0x0000FF;
inline constexpr uint32_t MediumBlue = 0x0000CD;
inline constexpr uint32_t DarkBlue = 0x00008B;
inline constexpr uint32_t Navy = 0x000080;
inline constexpr uint32_t MidnightBlue = 0x191970;
}

namespace brown {
inline constexpr uint32_t Cornsilk = 0xFFF8DC;
inline constexpr uint32_t BlanchedAlmond = 0xFFEBCD;
inline constexpr uint32_t Bisque = 0xFFE4C4;
inline constexpr uint32_t NavajoWhite = 0xFFDEAD;
inline constexpr uint32_t Wheat = 0xF5DEB3;
inline constexpr uint32_t BurlyWood = 0xDEB887;
inline constexpr uint32_t Tan = 0xD2B48C;
inline constexpr uint32_t RosyBrown = 0xBC8F8F;
inline constexpr uint32_t SandyBrown = 0xF4A460;
inline constexpr uint32_t Goldenrod = 0xDAA520;
inline constexpr uint32_t DarkGoldenrod = 0xB8860B;
inline constexpr uint32_t Peru = 0xCD853F;
inline constexpr uint32_t Chocolate = 0xD2691E;
inline constexpr uint32_t SaddleBrown = 0x8B4513;
inline constexpr uint32_t Sienna = 0xA0522D;
inline constexpr uint32_t Brown = 0xA52A2A;
inline constexpr uint32_t Maroon = 0x800000;
}

namespace white {
inline constexpr uint32_t White = 0xFFFFFF;
inline constexpr uint32_t Snow = 0xFFFAFA;
inline constexpr uint32_t HoneyDew = 0xF0FFF0;
inline constexpr uint32_t MintCream = 0xF5FFFA;
inline constexpr uint32_t Azure = 0xF0FFFF;
inline constexpr uint32_t AliceBlue = 0xF0F8FF;
inline constexpr uint32_t GhostWhite = 0xF8F8FF;
inline constexpr uint32_t WhiteSmoke = 0xF5F5F5;
inline constexpr uint32_t SeaShell = 0xFFF5EE;
inline constexpr uint32_t Beige = 0xF5F5DC;
inline constexpr uint32_t OldLace = 0xFDF5E6;
inline constexpr uint32_t FloralWhite = 0xFFFAF0;
inline constexpr uint32_t Ivory = 0xFFFFF0;
inline constexpr uint32_t AntiqueWhite = 0xFAEBD7;
inline constexpr uint32_t Linen = 0xFAF0E6;
inline constexpr uint32_t LavenderBlush = 0xFFF0F5;
inline constexpr uint32_t MistyRose = 0xFFE4E1;
}

namespace gray {
inline constexpr uint32_t Gainsboro = 0xDCDCDC;
inline constexpr uint32_t LightGray = 0xD3D3D3;
inline constexpr uint32_t Silver = 0xC0C0C0;
inline constexpr uint32_t DarkGray = 0xA9A9A9;
inline constexpr uint32_t Gray = 0x808080;
inline constexpr uint32_t DimGray = 0x696969;
inline constexpr uint32_t LightSlateGray = 0x778899;
inline constexpr uint32_t SlateGray = 0x708090;
inline constexpr uint32_t DarkSlateGray = 0x2F4F4F;
inline constexpr uint32_t Black = 0x000000;
}

}  // namespace rewind_viewer::colors
