#include "rewind_viewer.h"

#include <imgui/fontawesome.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>

#include <utility>

#include "version.h"

namespace {

inline bool key_modifier(const ImGuiIO &io) {
#ifdef __APPLE__
  return io.KeySuper;
#else
  return io.KeyCtrl;
#endif
}

}  // namespace

namespace rewind_viewer {

RewindViewer::RewindViewer(models::Config &config)
    : config_{config}
    , scene_{std::make_shared<models::Scene>(config_.scene, config_.ui->buffered_mode)} {
  uint16_t port = config_.network->start_port;
  servers_.emplace_back(
      std::make_unique<net::RewindServer>(scene_, config_.network->host, port++, true));

  for (int i = 0; i < config_.network->slave_connections; i++) {
    servers_.emplace_back(
        std::make_unique<net::RewindServer>(scene_, config_.network->host, port++, false));
  }

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigWindowsResizeFromEdges = false;

  setup_fonts();
  setup_style();
}

void RewindViewer::render() {
  // Handle keyboard before anything else
  handle_keys();

  auto [perma_frame, frame] = scene_->frames.get_frame(&ui_state_.current_frame_idx);
  current_frame_ = std::move(frame);

  main_menu_bar();
  status_overlay();
  playback_controls();
  shortcuts_help();
  frame_info();
  style_editor();
  ui_help();
  metrics();

  // Set viewport after all other windows size is known
  viewport();
}

void RewindViewer::stop() {
  for (auto &s : servers_) {
    s->stop();
  }
}

void RewindViewer::main_menu_bar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu(ICON_FA_EYE " View", true)) {
      ImGui::MenuItem("Status overlay", nullptr, &ui_state_.show_status_overlay);
      ImGui::MenuItem("Frame info", nullptr, &ui_state_.show_frame_info);
      if (ui_state_.developer_mode) {
        ImGui::Separator();
        ImGui::MenuItem("Style editor", nullptr, &ui_state_.show_style_editor);
        ImGui::MenuItem("Metrics", nullptr, &ui_state_.show_metrics);
        ImGui::MenuItem("UI Help", nullptr, &ui_state_.show_ui_help);
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(ICON_FA_CIRCLE_QUESTION " Help", true)) {
      ImGui::MenuItem(ICON_FA_KEYBOARD " Controls", nullptr, &ui_state_.show_shortcuts_help);
      ImGui::EndMenu();
    }
    ImGui::TextDisabled(ICON_FA_TAG " v%s", APP_VERSION);
    ImGui::EndMainMenuBar();

    ui_state_.main_menu_height = ImGui::GetFrameHeight();
  }
}

void RewindViewer::status_overlay() {
  if (!ui_state_.show_status_overlay) {
    return;
  }

  ImGui::SetNextWindowPos(ImVec2(5.0f, ui_state_.main_menu_height + 5.0f));
  const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

  if (ui_state_.show_status_overlay) {
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::Begin("FPS Overlay", nullptr, flags);
    ImGui::PopStyleVar(2);

    ImGui::BeginGroup();
    ImGui::TextColored({1.0, 1.0, 0.0, 1.0}, "FPS %.1f", ImGui::GetIO().Framerate);
    ImGui::SameLine();
    ImGui::Text("[%.1f ms]", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::EndGroup();
    std::string strstatus;
    ImVec4 color;
    static const float intensity = 1.0;
    for (const auto &server : servers_) {
      switch (server->get_state()) {
        case net::RewindServer::State::wait:
          strstatus = "WAITING";
          color = {intensity, intensity, 0.0, 1.0};
          break;
        case net::RewindServer::State::established:
          strstatus = "CONNECTED";
          color = {0.0, intensity, 0.0, 1.0};
          break;
        case net::RewindServer::State::closed:
          strstatus = "CLOSED";
          color = {intensity, 0.0, 0.0, 1.0};
          break;
      }
      ImGui::TextColored(color, ICON_FA_PLUG "%u: %s", server->get_port(), strstatus.c_str());
    }
    const ImVec4 mode_color = {0.3f, 0.0f, 0.0f, 1.000f};
    if (!config_.ui->buffered_mode) {
      ImGui::TextColored(mode_color, ICON_FA_CIRCLE_EXCLAMATION "Buffered mode disabled");
    }
    ImGui::End();
  }
}

void RewindViewer::frame_info() {
  if (!ui_state_.show_frame_info) {
    ui_state_.frame_info_width = 0.0f;
    return;
  }
  auto &io = ImGui::GetIO();
  ui_state_.frame_info_width = static_cast<float>(config_.ui->utility_width);
  ImGui::SetNextWindowPos(
      {io.DisplaySize.x - ui_state_.frame_info_width, ui_state_.main_menu_height}, ImGuiCond_None);
  ImGui::SetNextWindowSize(
      {ui_state_.frame_info_width,
       io.DisplaySize.y - ui_state_.main_menu_height - ui_state_.playback_controls_height},
      ImGuiCond_None);
  static const auto wflags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

  ImGui::Begin("Info", nullptr, wflags);
  const auto flags = ImGuiTreeNodeFlags_DefaultOpen;
  if (ImGui::CollapsingHeader(ICON_FA_GEARS " Settings")) {
    ImGui::TreePush("settings");
    if (ImGui::CollapsingHeader("Viewer", flags)) {
      ImGui::Checkbox("Close window by Escape key", &config_.ui->close_with_esc);
      ImGui::Checkbox("Update window when not in focus", &config_.ui->update_unfocused);
      if (ImGui::Checkbox("Buffered mode", &config_.ui->buffered_mode)) {
        scene_->frames.set_buffered_mode(config_.ui->buffered_mode);
      }
      ImGui::SameLine();
      ImGui::TextDisabled("(?)");
      if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(
            "Show frame only when it is finished. This option provides better performance.");
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
      }
    }
    if (ImGui::CollapsingHeader("UI", flags)) {
      if (ImGui::Combo("Theme", (int *)&config_.ui->style, "Light\0Dark\0ImGui Classic")) {
        setup_style();
      }
      ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoInputs);
      ImGui::ColorEdit3("Background", (float *)&config_.ui->canvas_background_color);
    }
    ImGui::TreePop();
  }
  if (ImGui::CollapsingHeader(ICON_FA_MAP " Map", flags)) {
    ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoInputs);
    ImGui::Checkbox("##show_grid", &config_.scene->show_grid);
    ImGui::SameLine();
    ImGui::ColorEdit3("Grid", (float *)&config_.scene->grid_color);

    ImGui::Checkbox("##draw_map_background", &config_.scene->show_background);
    ImGui::SameLine();
    ImGui::ColorEdit3("Canvas", (float *)&config_.scene->background_color);
    ImGui::Checkbox("Show game coordinates", &config_.scene->show_game_coordinates);
  }
  if (ImGui::CollapsingHeader(ICON_FA_CAMERA " Camera", flags)) {
    if (ImGui::Checkbox("Y axis up", &config_.scene->camera.y_axis_up)) {
      scene_->camera.set_y_axis_up(config_.scene->camera.y_axis_up);
    }
    ImGui::PushItemWidth(150);
    auto temp_position = scene_->camera.get_position();
    if (ImGui::InputFloat2("Position", (float *)&temp_position, "%.1f")) {
      scene_->camera.set_position(temp_position);
      ui_state_.ignore_frame_camera_viewport = true;
      ui_state_.selected_camera.clear();
    }
    float temp_scale = 1.0f / scene_->camera.get_scale();
    if (ImGui::InputFloat("Scale", &temp_scale, 0.05, 0.5, "%.2f")) {
      temp_scale = std::min(100.0f, std::max(0.01f, temp_scale));
      scene_->camera.set_scale(1.0f / temp_scale);
      ui_state_.ignore_frame_camera_viewport = true;
    }
    ImGui::PopItemWidth();

    if (ImGui::Button("Save")) {
      config_.scene->camera.position = scene_->camera.get_position();
      config_.scene->camera.scale = scene_->camera.get_scale();
    }
    ImGui::SameLine();
    if (ImGui::Button("Load")) {
      scene_->camera.set_position(config_.scene->camera.position);
      scene_->camera.set_scale(config_.scene->camera.scale);
      ui_state_.ignore_frame_camera_viewport = true;
      ui_state_.selected_camera.clear();
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(x:%.1f; y:%.1f) s:%.2f", config_.scene->camera.position.x,
                        config_.scene->camera.position.y, 1.0f / config_.scene->camera.scale);

    ImGui::Checkbox("Ignore client zoom", &ui_state_.ignore_frame_camera_viewport);
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted("Ignore provided by client viewport size for selected camera");
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
    }
    if (ImGui::BeginListBox("Frame Cameras",
                            ImVec2(-FLT_MIN, 3 * ImGui::GetTextLineHeightWithSpacing()))) {
      bool is_selected = ui_state_.selected_camera.empty();
      if (ImGui::Selectable("Free camera", is_selected)) {
        ui_state_.selected_camera.clear();
      }

      if (current_frame_) {
        const auto &cameras = current_frame_->get_cameras();
        for (const auto &[name, _] : cameras) {
          is_selected = ui_state_.selected_camera == name;
          if (ImGui::Selectable(name.c_str(), is_selected)) {
            ui_state_.selected_camera = name;
          }
        }
      }
      ImGui::EndListBox();
    }
  }
  if (ImGui::CollapsingHeader(ICON_FA_LIST " Layers visibility", flags)) {
    static const ImVec4 tick_button_colors[] = {
        ImVec4(0.5f, 0.5f, 0.5f, 1.0f),      // Disabled color
        ImVec4(0.38f, 0.741f, 0.229f, 1.0f)  // Enabled color
    };
    static const std::array<const char *, static_cast<size_t>(models::SceneConfig::LAYERS_COUNT)>
        tick_captions = {"##layer0", "##layer1", "##layer2", "##layer3", "##layer4",
                         "##layer5", "##layer6", "##layer7", "##layer8", "##layer9"};
    static const ImVec4 permanent_button_colors[] = {
        ImVec4(0.5f, 0.5f, 0.5f, 1.0f),      // Disabled color
        ImVec4(0.741f, 0.38f, 0.229f, 1.0f)  // Enabled color
    };
    static const std::array<const char *, static_cast<size_t>(models::SceneConfig::LAYERS_COUNT)>
        permanent_captions = {"##p_layer0", "##p_layer1", "##p_layer2", "##p_layer3", "##p_layer4",
                              "##p_layer5", "##p_layer6", "##p_layer7", "##p_layer8", "##p_layer9"};

    ImGui::Text("Frame layers");
    size_t idx = 0;
    for (bool &enabled : config_.scene->enabled_layers) {
      if (ImGui::ColorButton(tick_captions[idx], tick_button_colors[enabled],
                             ImGuiColorEditFlags_NoTooltip)) {
        enabled = !enabled;
      }
      if (++idx < models::SceneConfig::LAYERS_COUNT) {
        ImGui::SameLine();
      }
    }

    ImGui::Text("Permanent layers");
    idx = 0;
    for (bool &enabled : config_.scene->enabled_permanent_layers) {
      if (ImGui::ColorButton(permanent_captions[idx], permanent_button_colors[enabled],
                             ImGuiColorEditFlags_NoTooltip)) {
        enabled = !enabled;
      }
      if (++idx < models::SceneConfig::LAYERS_COUNT) {
        ImGui::SameLine();
      }
    }
  }
  if (ImGui::CollapsingHeader(ICON_FA_COMMENT " Frame message", flags)) {
    ImGui::BeginChild("FrameMsg", {0, 0}, true);
    if (current_frame_) {
      ImGui::TextWrapped("%s", current_frame_->get_user_message().c_str());
    }
    ImGui::EndChild();
  }

  config_.ui->utility_width = static_cast<int>(ImGui::GetWindowWidth());

  ImGui::End();
}

void RewindViewer::playback_controls() {
  if (!ui_state_.show_playback_controls) {
    ui_state_.playback_controls_height = 0.0f;
    return;
  }
  static const auto button_size = ImVec2{0, 0};
  static const float buttons_spacing = 5.0f;
  auto &io = ImGui::GetIO();
  float width = io.DisplaySize.x;
  ui_state_.playback_controls_height = 20 + 2 * ImGui::GetStyle().WindowPadding.y;
  ImGui::SetNextWindowPos({0, io.DisplaySize.y - ui_state_.playback_controls_height});
  ImGui::SetNextWindowSize({width, ui_state_.playback_controls_height});
  static const auto flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
  if (ImGui::Begin("Playback control", &ui_state_.show_playback_controls, flags)) {
    ImGui::BeginGroup();
    if (ImGui::Button(ICON_FA_BACKWARD_FAST "##fastprev", button_size)) {
      //    if (ImGui::IsItemActive()) { in case if long press will be needed
      if (ui_state_.current_frame_idx > config_.ui->fast_skip_speed) {
        ui_state_.current_frame_idx -= config_.ui->fast_skip_speed;
      } else {
        ui_state_.current_frame_idx = 0;
      }
      ui_state_.autoplay = false;
    }

    ImGui::SameLine(0.0f, buttons_spacing);
    if (ImGui::Button(ICON_FA_BACKWARD "##prev", button_size)) {
      if (ui_state_.current_frame_idx > 0) {
        ui_state_.current_frame_idx--;
      }
      ui_state_.autoplay = false;
    }

    ImGui::SameLine(0.0f, buttons_spacing);
    if (ui_state_.autoplay) {
      ui_state_.autoplay = !ImGui::Button(ICON_FA_PAUSE "##pause", button_size);
    } else {
      ui_state_.autoplay = ImGui::Button(ICON_FA_PLAY "##play", button_size);
    }

    ImGui::SameLine(0.0f, buttons_spacing);
    if (ImGui::Button(ICON_FA_FORWARD "##next", button_size)) {
      ui_state_.current_frame_idx++;
      ui_state_.autoplay = false;
    }

    ImGui::SameLine(0.0f, buttons_spacing);
    if (ImGui::Button(ICON_FA_FORWARD_FAST "##fastnext", button_size)) {
      ui_state_.current_frame_idx += config_.ui->fast_skip_speed;
      ui_state_.autoplay = false;
    }

    if (ui_state_.autoplay) {
      ui_state_.current_frame_idx++;
    }

    ImGui::SameLine();
    // Tick is one indexed
    const size_t frames_cnt = scene_->frames.size();
    if (frames_cnt > 0ul) {
      ui_state_.current_frame_idx =
          std::max(0ul, std::min(frames_cnt, ui_state_.current_frame_idx));
      ImGui::PushItemWidth(-1);
      const int frames_max = static_cast<int>(frames_cnt) - 1;
      int frame = static_cast<int>(ui_state_.current_frame_idx) + 1;
      const std::string slider_fmt = "%5d/" + std::to_string(frames_cnt);
      if (ImGui::SliderInt("##empty", &frame, 1, frames_max, slider_fmt.data(),
                           ImGuiSliderFlags_AlwaysClamp)) {
        ui_state_.current_frame_idx = frame - 1;
        ui_state_.autoplay = false;
      }
      ImGui::PopItemWidth();
    } else {
      ImGui::Text("Frame list empty");
    }

    ImGui::EndGroup();
    ImGui::End();
  }
}

void RewindViewer::shortcuts_help() {
  if (!ui_state_.show_shortcuts_help) {
    return;
  }
  ImGui::Begin(ICON_FA_KEYBOARD " Controls help", &ui_state_.show_shortcuts_help,
               ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::BulletText("Mouse drag on map to move camera");
  ImGui::BulletText("Mouse wheel to zoom");
  ImGui::BulletText("Space - play/stop frame playback");
  ImGui::BulletText(ICON_FA_ARROW_LEFT ", " ICON_FA_ARROW_RIGHT
                                       " - manually change frames\n"
                                       "press with [Ctrl/Cmd] to change faster");
  if (config_.ui->close_with_esc) {
    ImGui::BulletText("Esc - close application");
  }
  ImGui::BulletText("[Ctrl/Cmd] + R - Clear frame data");
  ImGui::BulletText("G - Toggle grid draw state");
  //  ImGui::BulletText("C - Switch between cameras");
  ImGui::BulletText("B - Toggle buffered draw mode");
  ImGui::BulletText("P - Show tooltip with cursor world coordinates");
  ImGui::BulletText("1-0 - Toggle layers visibility");
  ImGui::End();
}

void RewindViewer::viewport() {
  ImGuiIO &io = ImGui::GetIO();

  // Adjust viewport position and size to account for UI elements and padding
  ImVec2 viewport_pos = {
      1.0f * io.DisplayFramebufferScale.x,
      (ui_state_.playback_controls_height + 1.0f) * io.DisplayFramebufferScale.y};

  ImVec2 viewport_size = {
      (io.DisplaySize.x - ui_state_.frame_info_width - 2.f) * io.DisplayFramebufferScale.x,
      (io.DisplaySize.y - ui_state_.main_menu_height - ui_state_.playback_controls_height - 2.f) *
          io.DisplayFramebufferScale.y};

  // Adjust the mouse position based on UI
  auto mouse_pos = io.MousePos;
  mouse_pos.x -= 1.0f;
  mouse_pos.y -= (ui_state_.main_menu_height + 1.0f);
  mouse_pos.x *= io.DisplayFramebufferScale.x;
  mouse_pos.y *= io.DisplayFramebufferScale.y;

  // Set OpenGL viewport to the scaled and positioned area
  glViewport(static_cast<int>(viewport_pos.x), static_cast<int>(viewport_pos.y),
             static_cast<int>(viewport_size.x), static_cast<int>(viewport_size.y));

  // Correct the y-component of mouse coordinates to OpenGL space
  //  mouse_pos.y = viewport_size.y - mouse_pos.y;

  scene_->camera.set_viewport_size(viewport_size);
  bool set_camera = !ui_state_.selected_camera.empty();
  if (!io.WantCaptureMouse) {
    if (io.MouseWheel != 0.0f) {
      float zoom_factor = std::exp(-io.MouseWheel * config_.scene->camera.zoom_speed);
      scene_->camera.zoom(zoom_factor, mouse_pos);
      ui_state_.ignore_frame_camera_viewport = true;
    }

    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
      auto camera_move = io.MouseDelta;
      camera_move.x *= -io.DisplayFramebufferScale.x;
      camera_move.y *= -io.DisplayFramebufferScale.y;
      scene_->camera.move(camera_move);

      // Allow to move camera
      set_camera = false;
    }
  }
  if (set_camera) {
    // TODO: cleean up logic...
    if (current_frame_) {
      const auto &cameras = current_frame_->get_cameras();
      auto it = cameras.find(ui_state_.selected_camera);
      if (it != cameras.end()) {
        scene_->camera.set_view(it->second, ui_state_.ignore_frame_camera_viewport);
      }
    }
  }
  scene_->camera.update_projection();

  if (!io.WantCaptureMouse) {
    auto mouse_game_pos = scene_->camera.screen_to_game(mouse_pos);
    if (config_.scene->show_game_coordinates) {
      ImGui::BeginTooltip();
      ImGui::Text("(%.3f, %.3f)", mouse_game_pos.x, mouse_game_pos.y);
      ImGui::EndTooltip();
    }
    if (current_frame_) {
      auto popups = current_frame_->get_popups();
      for (size_t idx = 0; idx < popups->size(); ++idx) {
        if (!config_.scene->enabled_layers[idx]) {
          continue;
        }
        for (const auto &popup : (*popups)[idx]) {
          if (popup.hit_test(mouse_game_pos)) {
            ImGui::BeginTooltip();
            ImGui::Text("%s", popup.text());
            ImGui::EndTooltip();
          }
        }
      }
    }
  }
  scene_->render(ui_state_.current_frame_idx);

  // Cleanup opengl state
  glBindVertexArray(0);
  glUseProgram(0);
}

void RewindViewer::handle_keys() {
  const auto &io = ImGui::GetIO();

  if (!io.WantTextInput) {
    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_LeftArrow)) {
      if (key_modifier(io)) {
        if (ui_state_.current_frame_idx > config_.ui->fast_skip_speed) {
          ui_state_.current_frame_idx -= config_.ui->fast_skip_speed;
        } else {
          ui_state_.current_frame_idx = 0;
        }
      } else {
        if (ui_state_.current_frame_idx > 0) {
          ui_state_.current_frame_idx--;
        }
      }
      ui_state_.autoplay = false;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
      if (key_modifier(io)) {
        ui_state_.current_frame_idx += config_.ui->fast_skip_speed;
      } else {
        ui_state_.current_frame_idx++;
      }
      ui_state_.autoplay = false;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
      ui_state_.autoplay = !ui_state_.autoplay;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_G)) {
      config_.scene->show_grid = !config_.scene->show_grid;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_B)) {
      config_.ui->buffered_mode = !config_.ui->buffered_mode;
      scene_->frames.set_buffered_mode(config_.ui->buffered_mode);
    }
    if (ImGui::IsKeyPressed(ImGuiKey_P)) {
      config_.scene->show_game_coordinates = !config_.scene->show_game_coordinates;
    }
    if (config_.ui->close_with_esc && ImGui::IsKeyPressed(ImGuiKey_Escape)) {
      // TODO: should we call glfwSetWindowShouldClose(window, true); here or avoid extra deps?...
      ui_state_.close_requested = true;
    }
    if (ImGui::IsKeyChordPressed(ImGuiKey_D | ImGuiMod_Shortcut)) {
      ui_state_.developer_mode = true;
    }
    if (ImGui::IsKeyChordPressed(ImGuiKey_R | ImGuiMod_Shortcut)) {
      scene_->frames.clear();
    }

    // Layer toggle shortcuts
    auto &enabled_layers = config_.scene->enabled_layers;
    static const std::array<ImGuiKey, models::SceneConfig::LAYERS_COUNT> layer_shortcuts = {
        ImGuiKey_0, ImGuiKey_1, ImGuiKey_2, ImGuiKey_3, ImGuiKey_4,
        ImGuiKey_5, ImGuiKey_6, ImGuiKey_7, ImGuiKey_8, ImGuiKey_9,
    };
    for (size_t i = 0; i < layer_shortcuts.size(); ++i) {
      if (ImGui::IsKeyPressed(layer_shortcuts[i], false)) {
        enabled_layers[i] = !enabled_layers[i];
      }
    }
  }
}

void RewindViewer::style_editor() {
  if (!ui_state_.show_style_editor) {
    return;
  }
  ImGui::Begin("Style editor", &ui_state_.show_style_editor);
  ImGui::ShowStyleEditor();
  ImGui::End();
}

void RewindViewer::ui_help() {
  if (!ui_state_.show_ui_help) {
    return;
  }
  ImGui::Begin(ICON_FA_CIRCLE_INFO " UI guide", &ui_state_.show_ui_help,
               ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::ShowUserGuide();
  ImGui::End();
  ImGui::ShowDemoWindow();
}

void RewindViewer::metrics() {
  if (!ui_state_.show_metrics) {
    return;
  }
  ImGui::ShowMetricsWindow(&ui_state_.show_metrics);
}

void RewindViewer::setup_fonts() {
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
  for (const auto &font_file : config_.ui->font_files) {
    io.Fonts->AddFontFromFileTTF(font_file.c_str(), FONT_AWESOME_FONT_SIZE * scale_factor,
                                 &icons_config, icons_range);
  }

  io.FontGlobalScale = 1.0f / scale_factor;
  // Need to call it here, otherwise fontawesome glyph ranges would be corrupted on Windows
  ImGui_ImplOpenGL3_CreateDeviceObjects();
}

void RewindViewer::setup_style() {
  switch (config_.ui->style) {
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
  auto &style = ImGui::GetStyle();
  style.Alpha = 1.0f;
  style.FrameRounding = 4.0f;
  style.ChildRounding = 4;
  style.GrabRounding = 2;
  style.WindowBorderSize = 0.0f;
}

}  // namespace rewind_viewer
