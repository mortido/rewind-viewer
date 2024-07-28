cmake_minimum_required(VERSION 3.21)
project(rewind-viewer LANGUAGES C CXX)

#3rdparites
add_subdirectory(3rdparty/glad)
add_subdirectory(3rdparty/stb_image)
add_subdirectory(3rdparty/clsocket)
add_subdirectory(3rdparty/loguru)
add_subdirectory(3rdparty/rapidjson)
add_subdirectory(3rdparty/rapidyaml)
add_subdirectory(3rdparty/flatbuffers)
add_subdirectory(3rdparty/nfd)

#if(UNIX)
#    find_library(Glfw_LIBRARY glfw)
#else()
#    find_library(Glfw_LIBRARY glfw3)
#endif()
#if (Glfw_LIBRARY)
#    message(STATUS "Found glfw in system path: ${Glfw_LIBRARY}")
#    add_library(GLFW SHARED IMPORTED)
#    set_target_properties(GLFW PROPERTIES IMPORTED_LOCATION ${Glfw_LIBRARY})
#else()
#    message(STATUS "Build glfw from sources: ${Glfw_LIBRARY}")
#    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
#    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
#    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
#    add_subdirectory(3rdparty/glfw)
#endif()

if (EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -sMAX_WEBGL_VERSION=2 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -sMAX_WEBGL_VERSION=2 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/resources@/resources")
else()
    message(STATUS "Build glfw from sources: ${Glfw_LIBRARY}")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(3rdparty/glfw)
endif ()

set(GLM_TEST_ENABLE OFF)
add_subdirectory(3rdparty/glm)
add_subdirectory(3rdparty/imgui) # Depends from glm and GLFW


#Project
add_custom_target(CopyResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_BINARY_DIR}/resources
        COMMENT "Copying resources directory to build directory"
        )

#file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
# add_subdirectory(fbs)  # no need if messages pregenerated
add_subdirectory(src)
