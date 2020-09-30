cmake_minimum_required(VERSION 2.8.12)
project(uvvpis)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MACOSX_RPATH 1)

# Enable multithreaded compilation in MSVC
if (MSVC)
  add_definitions(/MP)
endif()

# Rewrite to a nicer output directory. Can be removed
set(ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output)
set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output)
set(RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output)

include_directories("${PROJECT_SOURCE_DIR}/include/tclap-1.2.2/include")

# Include Paths
# include_directories("${PROJECT_SOURCE_DIR}/include/PATH")

# Add fiiles to src
file(GLOB_RECURSE uvvpis_src 
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.cc"
    "${PROJECT_SOURCE_DIR}/src/*.c"
)
file(GLOB_RECURSE uvvpis_hdr
    "${PROJECT_SOURCE_DIR}/src/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/*.hh"
    "${PROJECT_SOURCE_DIR}/src/*.h"
)

# default flags to true
option(UVVPIS_CLI "BUILD EXECUTABLE" ON)


if (UVVPIS_CLI)
    add_executable(uvvpis ${uvvpis_src} ${uvvpis_hdr})
    target_link_libraries(uvvpis ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${ST_CXXFS_LIBS})
    target_include_directories(uvvpis PUBLIC ${PROJECT_SOURCE_DIR}/src)
endif ()