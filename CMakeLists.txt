cmake_minimum_required(VERSION 3.15)
project(TrueIssue)

set(CMAKE_CXX_STANDARD 17)

# MinHook
add_subdirectory(external/minhook)

# ImGui (собираем напрямую, так как нет CMakeLists.txt)
set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_dx11.cpp
    external/imgui/backends/imgui_impl_win32.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC external/imgui external/imgui/backends)

# Наш DLL
file(GLOB SOURCES src/*.cpp src/*.h)

add_library(TrueIssue SHARED ${SOURCES})
target_include_directories(TrueIssue PRIVATE external/imgui external/imgui/backends external/minhook/include)
target_link_libraries(TrueIssue PRIVATE imgui minhook d3d11.lib dxgi.lib)
