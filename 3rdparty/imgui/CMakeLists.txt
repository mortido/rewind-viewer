cmake_minimum_required(VERSION 3.21)
project(ImGui)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/**.h")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/**.h")
set(DEBUGGERS debuggers/imgui.gdb debuggers/imgui.natstepfilter debuggers/imgui.natvis)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${DEBUGGERS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE include/imgui)

#Dependency for implicit vector conversions
target_link_libraries(${PROJECT_NAME} glfw glm)

#get_target_property(glm_includes glm INTERFACE_INCLUDE_DIRECTORIES)
#target_include_directories(${PROJECT_NAME} PRIVATE ${glm_includes})
