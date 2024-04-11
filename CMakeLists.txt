cmake_minimum_required(VERSION 3.21)
project(rewind-viewer)

#3rdparites
add_subdirectory(3rdparty/glad)
add_subdirectory(3rdparty/stb_image)
add_subdirectory(3rdparty/clsocket)
add_subdirectory(3rdparty/loguru)

if(UNIX)
    find_library(Glfw_LIBRARY glfw)
else()
    find_library(Glfw_LIBRARY glfw3)
endif()
if (Glfw_LIBRARY)
    message(STATUS "Found glfw in system path: ${Glfw_LIBRARY}")
    add_library(GLFW SHARED IMPORTED)
    set_target_properties(GLFW PROPERTIES IMPORTED_LOCATION ${Glfw_LIBRARY})
else()
    message(STATUS "Build glfw from sources: ${Glfw_LIBRARY}")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(3rdparty/glfw)
endif()

set(GLM_TEST_ENABLE OFF)
add_subdirectory(3rdparty/glm)
add_subdirectory(3rdparty/imgui) # Depends from glm
add_subdirectory(3rdparty/flatbuffers)

#Project
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
# add_subdirectory(fbs)  # no need if messages pregenerated
add_subdirectory(src)
