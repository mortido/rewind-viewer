#include "ui/toolbox_panel.h"

#include <imgui/fontawesome.h>
#include <imgui/imgui.h>

namespace {
using namespace rewind_viewer::models;
void render_layers_buttons(
    const Scene &scene, const std::array<ImVec4, 2> button_colors,
    std::array<bool, static_cast<size_t>(Frame::LAYERS_COUNT)> &enabled_layers,
    const std::array<const char *, static_cast<size_t>(Frame::LAYERS_COUNT)> &captions,
    bool permanent) {
  for (size_t i = 0; i < Frame::LAYERS_COUNT; i++) {
    if (ImGui::ColorButton(captions[i], button_colors[enabled_layers[i]],
                           ImGuiColorEditFlags_NoTooltip)) {
      enabled_layers[i] = !enabled_layers[i];
    }
    // TODO: avoid str copy.
    auto name = scene.get_layer_name(i, permanent);
    if (!name.empty() && ImGui::BeginItemTooltip()) {
      ImGui::TextUnformatted(name.c_str());
      ImGui::EndTooltip();
    }
    ImGui::SameLine();
  }

  ImGui::TextDisabled("(?)");
  if (ImGui::BeginItemTooltip()) {
    ImGui::TextUnformatted(permanent ? "Permanent layers 1-10" : "Frame layers 1-10");
    ImGui::EndTooltip();
  }
}
}  // namespace

namespace rewind_viewer::ui {
void ToolboxPanel::render(RewindViewerState &ui_state, const models::Config &config,
                          models::Scene &scene,
                          std::vector<std::unique_ptr<gateway::ClientGateway>> &gateways) {
  if (!ui_state.show_toolbox_panel) {
    ui_state.frame_info_width = 0.0f;
    return;
  }
  auto &io = ImGui::GetIO();
  auto current_frame = scene.get_current_frame();

  ui_state.frame_info_width = static_cast<float>(config.ui->toolbox_width);
  ImGui::SetNextWindowPos({io.DisplaySize.x - ui_state.frame_info_width, ui_state.main_menu_height},
                          ImGuiCond_None);
  ImGui::SetNextWindowSize(
      {ui_state.frame_info_width,
       io.DisplaySize.y - ui_state.main_menu_height - ui_state.playback_controls_height},
      ImGuiCond_None);
  static const auto wflags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

  ImGui::Begin("Toolbox", nullptr, wflags);
  const auto flags = ImGuiTreeNodeFlags_DefaultOpen;
  if (ImGui::CollapsingHeader(ICON_FA_IMAGE " Canvas", flags)) {
    ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoInputs);
    ImGui::ColorEdit3("Background", (float *)&config.ui->canvas_background_color, ImGuiColorEditFlags_NoTooltip);
    ImGui::Checkbox("##show_grid", &config.scene->show_grid);
    ImGui::SameLine();
    ImGui::ColorEdit3("Grid", (float *)&config.scene->grid_color, ImGuiColorEditFlags_NoTooltip);

    ImGui::Checkbox("##draw_map_background", &config.scene->show_background);
    ImGui::SameLine();
    ImGui::ColorEdit3("Map", (float *)&config.scene->background_color, ImGuiColorEditFlags_NoTooltip);
    ImGui::Checkbox("Show game coordinates", &config.scene->show_game_coordinates);
  }
  if (ImGui::CollapsingHeader(ICON_FA_CAMERA " Camera", flags)) {
    ImGui::PushItemWidth(-1);
    if (ImGui::BeginCombo("##frame_cameras", ui_state.selected_camera.empty()
                                                 ? "none"
                                                 : ui_state.selected_camera.c_str())) {
      if (current_frame) {
        const auto &cameras = current_frame->get_cameras();
        for (const auto &[name, _] : cameras) {
          bool is_selected = ui_state.selected_camera == name;
          if (ImGui::Selectable(name.c_str(), is_selected)) {
            ui_state.selected_camera = name;
            ui_state.ignore_camera_zoom = false;
          }

          // Set the initial focus when opening the combo (scrolling to the selected item).
          if (is_selected) {
            ImGui::SetItemDefaultFocus();
          }
        }
      }
      ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    auto &camera = scene.get_camera();

    ImGui::PushItemWidth(70);
    float temp_scale = 1.0f / camera.get_scale();
    if (ImGui::InputFloat("Scale", &temp_scale, 0.0, 0.0, "%.2f")) {
      temp_scale = std::min(1000.0f, std::max(0.01f, temp_scale));
      camera.set_scale(1.0f / temp_scale);
      ui_state.ignore_camera_zoom = true;
    }
    ImGui::PopItemWidth();
    //    ImGui::SameLine(0, 80);
    //    if (ImGui::Button(ICON_FA_LOCATION_CROSSHAIRS " Re-center")) {
    //      // todo:scene.camera.set_position();
    //    }
    if (ImGui::Checkbox("Y axis up", &config.scene->camera.y_axis_up)) {
      camera.set_y_axis_up(config.scene->camera.y_axis_up);
    }
  }
  if (ImGui::CollapsingHeader(ICON_FA_LIST " Layers", flags)) {
    static const std::array<ImVec4, 2> tick_button_colors = {
        ImVec4(0.5f, 0.5f, 0.5f, 1.0f),      // Disabled color
        ImVec4(0.38f, 0.741f, 0.229f, 1.0f)  // Enabled color
    };
    static const std::array<const char *, static_cast<size_t>(models::SceneConfig::LAYERS_COUNT)>
        tick_captions = {"##layer0", "##layer1", "##layer2", "##layer3", "##layer4",
                         "##layer5", "##layer6", "##layer7", "##layer8", "##layer9"};
    static const std::array<ImVec4, 2> permanent_button_colors = {
        ImVec4(0.5f, 0.5f, 0.5f, 1.0f),      // Disabled color
        ImVec4(0.741f, 0.38f, 0.229f, 1.0f)  // Enabled color
    };
    static const std::array<const char *, static_cast<size_t>(models::SceneConfig::LAYERS_COUNT)>
        permanent_captions = {"##p_layer0", "##p_layer1", "##p_layer2", "##p_layer3", "##p_layer4",
                              "##p_layer5", "##p_layer6", "##p_layer7", "##p_layer8", "##p_layer9"};

    render_layers_buttons(scene, tick_button_colors, config.scene->enabled_layers, tick_captions,
                          false);
    render_layers_buttons(scene, permanent_button_colors, config.scene->enabled_permanent_layers,
                          permanent_captions, true);
  }
  if (ui_state.show_control_pad_window) {
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
    if (ImGui::Begin(ICON_FA_GAMEPAD " Control Pad", &ui_state.show_control_pad_window,
                     ImGuiWindowFlags_AlwaysAutoResize)) {
      control_pad_.render(gateways);
      ImGui::End();
    }
  } else {
    if (ImGui::Button(ICON_FA_WINDOW_MAXIMIZE)) {
      ui_state.show_control_pad_window = true;
    }
    ImGui::SameLine();
    if (ImGui::CollapsingHeader(ICON_FA_GAMEPAD " Control Pad", flags)) {
      ImGui::BeginChild("##control_pad", {0, 0},
                        ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);
      control_pad_.render(gateways);
      ImGui::EndChild();
    }
  }
  if (ImGui::CollapsingHeader(ICON_FA_COMMENT " Frame message", flags)) {
    ImGui::BeginChild("FrameMsg", {0, 0}, ImGuiChildFlags_Border);
    if (current_frame) {
      ImGui::TextWrapped("%s", current_frame->get_user_message().c_str());
    }
    ImGui::EndChild();
  }

  config.ui->toolbox_width = static_cast<int>(ImGui::GetWindowWidth());

  ImGui::End();
}

}  // namespace rewind_viewer::ui