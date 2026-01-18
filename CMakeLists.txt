cmake_minimum_required(VERSION 3.10)
project(CAD)

# Set C++ standard (optional)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Place all executables in <build_dir>/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Place all libraries (.lib) in <build_dir>/lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Place all shared libraries (.dll / .so) in <build_dir>/bin
# (On Windows, DLLs must be in the same folder as the EXE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(CTest)

# Add subdirectory for source files
add_subdirectory(src)