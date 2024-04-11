cmake_minimum_required(VERSION 3.21)
project(cpp_example)

add_subdirectory(clsocket)
add_subdirectory(flatbuffers)
add_subdirectory(rewind_viewer)

set(SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE rewind_viewer_client)
