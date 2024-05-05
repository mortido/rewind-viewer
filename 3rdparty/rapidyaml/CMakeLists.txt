cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(rapidyaml LANGUAGES CXX)

set(SOURCES src/ryml.cpp)
set(HEADERS include/rapidyaml/ryml.hpp)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
target_compile_definitions(${PROJECT_NAME} PUBLIC -DRYML_SINGLE_HEADER)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE include/rapidyaml)
