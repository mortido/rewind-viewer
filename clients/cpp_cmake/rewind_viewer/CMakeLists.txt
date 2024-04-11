cmake_minimum_required(VERSION 3.21)
project(rewind_viewer_client)

add_library(${PROJECT_NAME} INTERFACE)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(rewind_viewer_client INTERFACE flatbuffers clsocket)
