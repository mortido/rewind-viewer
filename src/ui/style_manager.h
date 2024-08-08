#pragma once

#include <imgui/imgui.h>

#include <vector>

#include "models/config.h"

namespace rewind_viewer::ui {
class StyleManager {
 public:
  struct ColorScheme {
    ImVec4 ok_accent;
    ImVec4 warn_accent;
    ImVec4 error_accent;
  };

  ImFont* draw_font;

 private:
  constexpr static ColorScheme kLightThemeColors = {
      {0.0f, 0.0f, 0.8f, 1.0f}, {0.6f, 0.0f, 0.6f, 1.0f}, {0.8f, 0.0f, 0.0f, 1.0f}};

  constexpr static ColorScheme kDarkThemeColors = {
      {0.2f, 1.0f, 0.2f, 1.0f}, {1.0f, 1.0f, 0.2f, 1.0f}, {1.0f, 0.3f, 0.2f, 1.0f}};

  constexpr static ColorScheme kClassicThemeColors = {
      {0.2f, 1.0f, 0.2f, 1.0f}, {1.0f, 1.0f, 0.2f, 1.0f}, {1.0f, 0.3f, 0.2f, 1.0f}};
  ColorScheme current_scheme_;

 public:
  void setup_fonts(const models::UIConfig& ui_config);
  void setup_style(models::UIStyle style);
  const ColorScheme& get_color_scheme() const { return current_scheme_; };
};

}  // namespace rewind_viewer::ui
