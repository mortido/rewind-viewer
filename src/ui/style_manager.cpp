
#include "ui/style_manager.h"

#include <imgui/fontawesome.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>

namespace rewind_viewer::ui {
void StyleManager::setup_fonts(const std::vector<std::string> &font_files) const {
  ImGuiIO &io = ImGui::GetIO();

  const float scale_factor = std::max(io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
  auto font_cfg = ImFontConfig();
  font_cfg.SizePixels = DEFAULT_FONT_SIZE * scale_factor;
  font_cfg.OversampleH = 1;
  font_cfg.OversampleV = 1;
  font_cfg.PixelSnapH = true;
  io.Fonts->AddFontDefault(&font_cfg);

  // Load and merge fontawesome to current font
  const ImWchar icons_range[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
  ImFontConfig icons_config;
  icons_config.MergeMode = true;
  icons_config.PixelSnapH = true;
  icons_config.GlyphMinAdvanceX = FONT_AWESOME_FONT_SIZE * scale_factor;
  icons_config.GlyphOffset = ImVec2{0, 2.0};
  for (const auto &font_file : font_files) {
    io.Fonts->AddFontFromFileTTF(font_file.c_str(), FONT_AWESOME_FONT_SIZE * scale_factor,
                                 &icons_config, icons_range);
  }

  io.FontGlobalScale = 1.0f / scale_factor;
  // Need to call it here, otherwise fontawesome glyph ranges would be corrupted on Windows
  ImGui_ImplOpenGL3_CreateDeviceObjects();
}

void StyleManager::setup_style(models::UIStyle style) const{
  switch (style) {
    case models::UIStyle::light: {
      ImGui::StyleColorsLight();
      break;
    }
    case models::UIStyle::dark: {
      ImGui::StyleColorsDark();
      break;
    }
    case models::UIStyle::classic: {
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
  imgui_style.WindowPadding = {4.0,4.0};
//  imgui_style.FramePadding = {4.0,4.0};
//  imgui_style.ItemSpacing = {4.0,2.0};
}

}  // namespace rewind_viewer::ui
