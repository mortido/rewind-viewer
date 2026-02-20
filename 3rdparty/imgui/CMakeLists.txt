cmake_minimum_required(VERSION 3.21)
project(ImGui)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/**.h")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/**.h")
set(DEBUGGERS debuggers/imgui.gdb debuggers/imgui.natstepfilter debuggers/imgui.natvis)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${DEBUGGERS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE include/imgui)

#Dependency for implicit vector conversions
target_include_directories(${PROJECT_NAME} PRIVATE
        $<TARGET_PROPERTY:glm,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>
)

find_package(Freetype REQUIRED)
if (TARGET Freetype::Freetype)
    target_link_libraries(${PROJECT_NAME} PRIVATE lunasvg Freetype::Freetype)
else()
    target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE lunasvg ${FREETYPE_LIBRARIES})
endif()

#get_target_property(glm_includes glm INTERFACE_INCLUDE_DIRECTORIES)
#target_include_directories(${PROJECT_NAME} PRIVATE ${glm_includes})
