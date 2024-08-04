
#include "ui/main_menu.h"

#include <imgui/fontawesome.h>
#include <imgui/imgui.h>
#include <nfd/nfd.h>

#include "gateway/transport/dump_reader.h"
#include "version.h"

namespace rewind_viewer::ui {
void MainMenu::render(RewindViewerState& ui_state, const models::Config& config,
                      models::Scene& scene, gateway::ClientGateway& master_gateway, const StyleManager& style_manager) {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu(ICON_FA_FILE " File", true)) {
      if (ImGui::MenuItem("Open")) {
        std::string file_path = open_file_dialog();
        if (!file_path.empty()) {
          ui_state.current_frame_idx = 0;
          master_gateway.substitute_transport(std::make_shared<gateway::DumpReader>(file_path));
        }
      }
//      if (ImGui::MenuItem("Save ...")) {
//        std::string file_path = save_file_dialog();
//        // TODO: Save replays
//      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(ICON_FA_GEARS " Settings", true)) {
      ImGui::MenuItem("Close window by Escape key", nullptr, &config.ui->close_with_esc);
      ImGui::MenuItem("Update window when not in focus", nullptr, &config.ui->update_unfocused);
      if (ImGui::MenuItem("Buffered mode", nullptr, &config.ui->buffered_mode)) {
        scene.set_buffered_mode(config.ui->buffered_mode);
      }
      if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(
            "Show frame only when it is finished. This option provides better performance.");
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
      }
      if (ImGui::BeginMenu("Theme", true)) {
        if (ImGui::MenuItem("Light", nullptr, config.ui->style == models::UIStyle::light)) {
          config.ui->style = models::UIStyle::light;
          style_manager.setup_style(models::UIStyle::light);
        }
        if (ImGui::MenuItem("Dark", nullptr, config.ui->style == models::UIStyle::dark)) {
          config.ui->style = models::UIStyle::dark;
          style_manager.setup_style(models::UIStyle::dark);
        }
        if (ImGui::MenuItem("ImGui Classic", nullptr,
                            config.ui->style == models::UIStyle::classic)) {
          config.ui->style = models::UIStyle::classic;
          style_manager.setup_style(models::UIStyle::classic);
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(ICON_FA_EYE " View", true)) {
      ImGui::MenuItem("Status", nullptr, &ui_state.show_status_overlay);
      ImGui::MenuItem("Toolbox", nullptr, &ui_state.show_toolbox_panel);
      if (ui_state.developer_mode) {
        ImGui::Separator();
        ImGui::MenuItem("Style editor", nullptr, &ui_state.show_style_editor);
        ImGui::MenuItem("Metrics", nullptr, &ui_state.show_metrics);
        ImGui::MenuItem("UI Help", nullptr, &ui_state.show_ui_help);
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(ICON_FA_CIRCLE_QUESTION " Help", true)) {
      ImGui::MenuItem(ICON_FA_KEYBOARD " Hotkeys", nullptr, &ui_state.show_shortcuts_help);
      ImGui::EndMenu();
    }
    ImGui::TextDisabled(ICON_FA_TAG " v%s", APP_VERSION);
    ImGui::EndMainMenuBar();

    ui_state.main_menu_height = ImGui::GetFrameHeight();
  }
}

std::string MainMenu::open_file_dialog() {
  nfdchar_t* out_path = nullptr;
  nfdresult_t result = NFD_OpenDialog("rwn", nullptr, &out_path);

  if (result == NFD_OKAY) {
    std::string file_path(out_path);
    free(out_path);  // remember to free the allocated memory
    return file_path;
  } else if (result == NFD_CANCEL) {
    return "";
  } else {
    LOG_ERROR("Error opening file: %s", NFD_GetError());
    return "";
  }
}

std::string MainMenu::save_file_dialog() {
  nfdchar_t* out_path = nullptr;
  nfdresult_t result = NFD_SaveDialog("rwn", nullptr, &out_path);

  if (result == NFD_OKAY) {
    std::string file_path(out_path);
    free(out_path);  // remember to free the allocated memory
    return file_path;
  } else if (result == NFD_CANCEL) {
    return "";
  } else {
    LOG_ERROR("Error opening file: %s", NFD_GetError());
    return "";
  }
}
}  // namespace rewind_viewer::ui