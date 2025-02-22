
#include "ui/main_menu.h"

#include <imgui/fontawesome.h>
#include <imgui/imgui.h>
#include <nfd/nfd.h>

#include "gateway/transport/dump_reader.h"
#include "version.h"

namespace rewind_viewer::ui {
void MainMenu::render(RewindViewerState& ui_state, const models::Config& config,
                      models::Scene& scene,
                      std::vector<std::unique_ptr<gateway::ClientGateway>>& gateways,
                      StyleManager& style_manager) {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu(ICON_FA_FILE " File", true)) {
      if (ImGui::MenuItem("Open")) {
        std::string file_path = open_file_dialog();
        if (!file_path.empty()) {
          scene.set_current_frame_idx(0ul);
          gateways[0]->substitute_transport(std::make_shared<gateway::DumpReader>(file_path));
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
    ImGui::Dummy(ImVec2(100.0f, 0.0f));
    ImGui::Separator();

    const auto& colors=style_manager.get_color_scheme();
    ImGui::TextColored(colors.warn_accent, "FPS %.1f [%.1f ms]", ImGui::GetIO().Framerate,
                       1000.0f / ImGui::GetIO().Framerate);


    if (!config.ui->buffered_mode) {
      ImGui::TextColored(colors.error_accent, ICON_FA_TRIANGLE_EXCLAMATION "Buffered mode disabled");
    }

    std::string str_status;
    const ImVec4* color;
    for (const auto& gateway : gateways) {
      switch (gateway->get_state()) {
        case gateway::ClientGateway::State::wait:
          str_status = "WAITING";
          color = &colors.warn_accent;
          break;
        case gateway::ClientGateway::State::established:
          str_status = "CONNECTED";
          color = &colors.ok_accent;
          break;
        case gateway::ClientGateway::State::aborting:
        case gateway::ClientGateway::State::closed:
          str_status = gateway->get_state() == gateway::ClientGateway::State::aborting
                           ? "ABORTING"
                           : "CLOSED";
          color = &colors.error_accent;
          break;
        default:
          str_status = "UNKNOWN";
          color = &colors.error_accent;
      }

      ImGui::TextColored(*color, ICON_FA_PLUG " %s: %s", gateway->get_name().c_str(),
                         str_status.c_str());
    }

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