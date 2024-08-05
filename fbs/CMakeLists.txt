cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)

# Find the FlatBuffers compiler (flatc).
find_program(FLATC_EXECUTABLE NAMES flatc)

# Define the input and output files for FlatBuffers.
set(FBS_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/rewind_events.fbs"
        "${CMAKE_CURRENT_SOURCE_DIR}/client_request.fbs"
        "${CMAKE_CURRENT_SOURCE_DIR}/vector2f.fbs"
        )
set(INCLUDE_FBS_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(GENERATED_FBS_DIR "${INCLUDE_FBS_DIR}/messages")
file(MAKE_DIRECTORY ${GENERATED_FBS_DIR})
set(GENERATED_FBS_HDRS
        "${GENERATED_FBS_DIR}/rewind_events.fbs.h"
        "${GENERATED_FBS_DIR}/rewind_message.fbs.h"
        "${GENERATED_FBS_DIR}/vector2f.fbs.h"
        )

# Custom command to compile the FlatBuffers schema file to C++ header.
add_custom_command(
        OUTPUT ${GENERATED_FBS_HDRS}
        COMMAND ${FLATC_EXECUTABLE} -o ${GENERATED_FBS_DIR} --filename-suffix .fbs --cpp ${FBS_SRC}
        DEPENDS ${FBS_SRC}
        COMMENT "Compiling ${FBS_SRC} to C++ header with FlatBuffers"
)

# Define an INTERFACE library for the generated FlatBuffers code.
add_library(flatbuffers_messages INTERFACE)
target_link_libraries(flatbuffers_messages INTERFACE flatbuffers)
target_include_directories(flatbuffers_messages INTERFACE ${INCLUDE_FBS_DIR})

## Since INTERFACE libraries do not have build output,
## manually specify that this target depends on the generated headers.
add_custom_target(flatbuffers_messages_sources DEPENDS ${GENERATED_FBS_HDRS})
add_dependencies(flatbuffers_messages flatbuffers_messages_sources)
