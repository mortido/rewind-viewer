cmake_minimum_required(VERSION 3.21)
project(ImGui)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/**.h")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/**.h")
set(DEBUGGERS debuggers/imgui.gdb debuggers/imgui.natstepfilter debuggers/imgui.natvis)
set(FREETYPE_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/misc/imgui_freetype.cpp")

option(IMGUI_USE_FREETYPE "Enable FreeType/lunasvg support for ImGui font building" ON)
if (IMGUI_USE_FREETYPE)
    find_package(Freetype QUIET)
endif()

if (NOT (IMGUI_USE_FREETYPE AND Freetype_FOUND))
    list(REMOVE_ITEM SOURCES "${FREETYPE_SOURCE}")
    if (IMGUI_USE_FREETYPE)
        message(WARNING "Freetype not found, building ImGui without FreeType support.")
    endif()
endif()

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${DEBUGGERS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE include/imgui)

# Dependency for implicit vector conversions
target_include_directories(${PROJECT_NAME} PRIVATE
        $<TARGET_PROPERTY:glm,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>
)

if (IMGUI_USE_FREETYPE AND Freetype_FOUND)
    target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_USE_FREETYPE)
    if (TARGET Freetype::Freetype)
        target_link_libraries(${PROJECT_NAME} PRIVATE lunasvg Freetype::Freetype)
    else()
        target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME} PRIVATE lunasvg ${FREETYPE_LIBRARIES})
    endif()
endif()

#get_target_property(glm_includes glm INTERFACE_INCLUDE_DIRECTORIES)
#target_include_directories(${PROJECT_NAME} PRIVATE ${glm_includes})
