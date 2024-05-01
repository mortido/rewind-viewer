cmake_minimum_required(VERSION 3.21)
project(rewind-viewer-executable)
set(APP_NAME "rewind-viewer")

add_compile_options(-Wall)
if (MSVC)
    add_compile_options(/W0)
else()
    add_compile_options(-Wextra)
endif()

set(SOURCES
    main.cpp
    rewind_viewer.cpp

    models/camera.cpp
    models/frame.cpp
    models/Popup.cpp

    net/tcp_server.cpp
    net/rewind_server.cpp

    render/Shader.cpp
    render/utils.cpp
    render/ResourceManager.cpp
    render/RenderContext.cpp
    render/Renderer.cpp
)

#set(MACOSX_BUNDLE_BUNDLE_NAME "MyApp")
#set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.myapp")
#set(MACOSX_BUNDLE_INFO_STRING "My Application")
#set(MACOSX_BUNDLE_ICON_FILE "MyAppIcon.icns")
#set(MACOSX_BUNDLE_LONG_VERSION_STRING "Version 1.0.0")
#set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")
#set(MACOSX_BUNDLE_BUNDLE_VERSION "1.0")
#
#add_executable(${APP_NAME} MACOSX_BUNDLE ${Sources})

add_executable(${APP_NAME} ${SOURCES})
target_compile_features(${APP_NAME} PUBLIC cxx_std_17)
target_include_directories(${APP_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_definitions(${APP_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_link_libraries(${APP_NAME} Glad glfw glm ImGui stb_image clsocket loguru flatbuffers rapidyaml)

set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${APP_NAME} PRIVATE OPENGL_DEBUG)
endif()

