#pragma once

#include <vector>

#include "models/config.h"

namespace rewind_viewer::ui {
class StyleManager {
 private:
  constexpr static float DEFAULT_FONT_SIZE = 13.0f;
  constexpr static float FONT_AWESOME_FONT_SIZE = 14.0f;

 public:
  void setup_fonts(const std::vector<std::string> &font_files) const;
  void setup_style(models::UIStyle style) const;
};

}  // namespace rewind_viewer::ui
