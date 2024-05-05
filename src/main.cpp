#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "common/logger.h"
#include "gl/opengl.h"
#include "rewind_viewer.h"

// #include <stb_image.h>

constexpr const char* CONFIG_FILENAME = "rewind-viewer.yaml";
constexpr const char* LOG_FILENAME = "rewind-viewer.log";
constexpr const char* WINDOW_TITLE = "Rewind Viewer [mortido remix]";

GLFWwindow* create_window(const rewind_viewer::models::UIConfig& config) {
  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac

#ifdef OPENGL_DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

#if !defined(__APPLE__)
#if (GL_ARB_debug_output)
  LOG_INFO("OpenGL:: Debug output enabled");
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
  glDebugMessageCallbackARB(cg::debug_output_callback, nullptr);
  glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
#endif
#endif

  GLFWwindow* window =
      glfwCreateWindow(config.windows_width, config.windows_height, WINDOW_TITLE, nullptr, nullptr);
  if (window == nullptr) {
    return window;
  }

  //  int width;
  //  int height;
  //  int nr_channels;
  //  const std::string icon_path = "resources/icon.png";
  //  auto icon_data = stbi_load(icon_path.c_str(), &width, &height, &nr_channels, 0);
  //  if (!icon_data) {
  //    LOG_ERROR(
  //        "Cannot find application icon (%s). "
  //        "Make sure you launch viewer from directory with 'resources' folder. ",
  //        icon_path.c_str());
  //    return nullptr;
  //  }
  //  GLFWimage icon{width, height, icon_data};
  //  LOG_INFO("Setup application icon");
  //  glfwSetWindowIcon(window, 1, &icon);

  glfwMakeContextCurrent(window);
  LOG_INFO("Enable vsync");
  glfwSwapInterval(1);

  LOG_INFO("Setup Dear ImGui context");
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  LOG_INFO("Setup Platform/Renderer backends");
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  return window;
}

static void glfw_error_callback(int error, const char* description) {
  LOG_FATAL("GLFW Error %d: %s", error, description);
}

int main(int argc, char** argv) {
  loguru::g_stderr_verbosity = loguru::Verbosity_INFO;
  loguru::init(argc, argv);
  loguru::add_file(LOG_FILENAME, loguru::Truncate, loguru::g_stderr_verbosity);

  LOG_INFO("Load configuration from %s", CONFIG_FILENAME);
  auto& config = rewind_viewer::models::Config::get_instance();
  // TODO: config.load_from_file(CONFIG_FILENAME);

  glfwSetErrorCallback(glfw_error_callback);
  LOG_INFO("Initialize GLFW");
  if (!glfwInit()) {
    LOG_FATAL("Failed to initialize GLFW");
    return EXIT_FAILURE;
  }

  LOG_INFO("Create main window");
  auto window = create_window(config.ui);
  if (!window) {
    LOG_FATAL("Cannot setup window");
    return EXIT_FAILURE;
  }

  LOG_INFO("Load OpenGL functions");
  if (!gladLoadGL()) {
    LOG_FATAL("Failed to load opengl");
    return EXIT_FAILURE;
  }

  LOG_INFO("OpenGL %s, GLSL %s", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
  LOG_INFO("Driver %s, Renderer %s", glGetString(GL_VENDOR), glGetString(GL_RENDERER));

  rewind_viewer::RewindViewer rewind(config);

  glEnable(GL_BLEND);
//  glEnable(GL_STENCIL_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  while (!glfwWindowShouldClose(window)) {
    // Poll and handle events
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glClearColor(config.ui.canvas_background_color.r, config.ui.canvas_background_color.g,
                 config.ui.canvas_background_color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    rewind.render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  LOG_INFO("Stop rewind");
  rewind.stop();

  LOG_INFO("Clean up");
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  LOG_INFO("Exit from application");
  return EXIT_SUCCESS;
}
