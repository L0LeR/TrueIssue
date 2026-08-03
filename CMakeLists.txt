cmake_minimum_required(VERSION 3.15)
project(TrueIssue)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/imgui)
add_subdirectory(external/minhook)

file(GLOB SOURCES src/*.cpp src/*.h)

add_library(TrueIssue SHARED ${SOURCES})
target_include_directories(TrueIssue PRIVATE external/imgui external/imgui/backends external/minhook/include)
target_link_libraries(TrueIssue PRIVATE imgui minhook d3d11.lib dxgi.lib)
