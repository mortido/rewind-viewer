
#include "ui/style_manager.h"

#include <imgui/fontawesome.h>
#include <imgui/imgui.h>
#include <imgui/imgui_freetype.h>
#include <imgui/imgui_impl_opengl3.h>

namespace rewind_viewer::ui {
void StyleManager::setup_fonts(const models::UIConfig &ui_config) {
  ImGuiIO &io = ImGui::GetIO();

  const float scale_factor = std::max(io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
  const float font_size = ui_config.font_size_pixels * scale_factor;
  const ImWchar icons_range[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
  const ImWchar emoji_range[] = {0x1, 0x1FFFF, 0};

  io.Fonts->AddFontFromFileTTF(ui_config.default_font.c_str(), font_size);

  //   Load and merge fontawesome to current font
  ImFontConfig icon_config;
  icon_config.MergeMode = true;
  icon_config.OversampleH = 1;
  icon_config.OversampleV = 1;
  icon_config.PixelSnapH = true;
  icon_config.GlyphMinAdvanceX = font_size;
  icon_config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;

  io.Fonts->AddFontFromFileTTF(ui_config.font_awesome_regular.c_str(), font_size, &icon_config,
                               icons_range);
  io.Fonts->AddFontFromFileTTF(ui_config.font_awesome_solid.c_str(), font_size, &icon_config,
                               icons_range);
  io.Fonts->AddFontFromFileTTF(ui_config.emoji_font.c_str(), font_size, &icon_config, emoji_range);
  io.Fonts->Build();

  draw_font =
      io.Fonts->AddFontFromFileTTF(ui_config.default_font.c_str(), ui_config.draw_font_size_pixels);
  ImFontConfig emoji_config;
  emoji_config.MergeMode = true;
  emoji_config.OversampleH = 1;
  emoji_config.OversampleV = 1;
  emoji_config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;

  io.Fonts->AddFontFromFileTTF(ui_config.emoji_font.c_str(), ui_config.draw_font_size_pixels,
                               &emoji_config, emoji_range);
  io.Fonts->Build();

  //  io.FontGlobalScale = 1.0f / scale_factor;
  //   Need to call it here, otherwise fontawesome glyph ranges would be corrupted on Windows
  //  ImGui_ImplOpenGL3_CreateDeviceObjects();
}

void StyleManager::setup_style(models::UIStyle style) {
  switch (style) {
    case models::UIStyle::light: {
      current_scheme_ = kLightThemeColors;
      ImGui::StyleColorsLight();
      break;
    }
    case models::UIStyle::dark: {
      current_scheme_ = kDarkThemeColors;
      ImGui::StyleColorsDark();
      break;
    }
    case models::UIStyle::classic: {
      current_scheme_ = kClassicThemeColors;
      ImGui::StyleColorsClassic();
      break;
    }
    default: {
      LOG_WARN("Incorrect style");
    }
  }

  // Custom changes
  auto &imgui_style = ImGui::GetStyle();
  imgui_style.Alpha = 1.0f;
  imgui_style.FrameRounding = 4.0f;
  imgui_style.ChildRounding = 4;
  imgui_style.GrabRounding = 2;
  imgui_style.WindowBorderSize = 0.0f;
  imgui_style.WindowPadding = {4.0, 4.0};
  imgui_style.FramePadding = {4.0, 2.0};
  imgui_style.ItemSpacing = {4.0, 4.0};
}

}  // namespace rewind_viewer::ui
