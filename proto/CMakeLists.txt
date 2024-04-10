cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)
message("Protobuf_LIBRARIES: " ${Protobuf_LIBRARIES})
message("Protobuf_INCLUDE_DIRS: " ${Protobuf_INCLUDE_DIRS})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS rewind_messages.proto)
add_library(proto_messages STATIC ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(proto_messages PUBLIC ${PROTOBUF_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(proto_messages PUBLIC ${Protobuf_LIBRARIES})
