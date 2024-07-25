set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

# Specify the cross compiler
set(CMAKE_C_COMPILER "emcc")
set(CMAKE_CXX_COMPILER "em++")

# Specify the ar tool
set(CMAKE_AR "emar")
set(CMAKE_RANLIB "emranlib")

# Specify additional compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_SDL=2")

# Set the output directory for the generated files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")