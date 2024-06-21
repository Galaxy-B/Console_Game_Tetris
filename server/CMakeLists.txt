cmake_minimum_required(VERSION 3.15)
project(My_Server)

set(CMAKE_CXX_STANDARD 17)

# set makefile generator
# set(CMAKE_GENERATOR "MinGW Makefiles")
# set(CMAKE_MAKE_PROGRAM "D:/vscode/mingw64/bin/make")

# don't generate clangd commands
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

# redirect the build output to a subdirectory of build
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# redirect the binary program to output directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)

# link ws2_32 to support socket 
link_libraries(ws2_32)

include_directories(include)

aux_source_directory(src SRC_LIST)

add_executable(My_Server ${SRC_LIST})
