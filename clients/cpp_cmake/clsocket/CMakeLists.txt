cmake_minimum_required(VERSION 3.21)
project(clsocket)

set(SOURCES
        src/SimpleSocket.cpp
        src/ActiveSocket.cpp
        src/PassiveSocket.cpp
        )
set(HEADERS
        include/clsocket/ActiveSocket.h
        include/clsocket/Host.h
        include/clsocket/PassiveSocket.h
        include/clsocket/SimpleSocket.h
        include/clsocket/StatTimer.h
        )

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC include/clsocket)

# OS and compiler checks.
if(UNIX)
    # linux / normal unix
    add_definitions(-D_LINUX)
    if(CYGWIN)
        # Special Cygwin stuff here
    elseif(APPLE)
        # Special Apple stuff here
        remove_definitions(-D_LINUX)
        add_definitions(-D_DARWIN)
    endif()
elseif(WIN32)
    add_definitions(-DWIN32)
    SET(PROJECT_LIBS Ws2_32.lib)
    if(MINGW)
        # Special MINGW stuff here
    elseif(MSVC)
        # Special MSVC stuff here
        add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
    else()
        # No idea what it is, but there's a good chance it's too weird.
        MESSAGE( FATAL_ERROR "Using unknown WIN32 compiler... Please add to build system." )
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIBS})
