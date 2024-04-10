cmake_minimum_required(VERSION 3.21)
project(loguru)

set(SOURCES src/loguru.cpp)
set(HEADERS include/loguru/loguru.hpp)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC include/loguru)

if (UNIX)
    find_package(Threads REQUIRED) # defines IMPORTED target Threads::Threads
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            Threads::Threads    # pthreads (or equivalent)
            ${_lib_dl_linkflag} # dl (or equivalent)
            )
#    target_link_libraries(${PROJECT_NAME} PRIVATE dl pthread)
endif ()
