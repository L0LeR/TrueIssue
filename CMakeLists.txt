cmake_minimum_required(VERSION 3.15)
project(TrueIssue)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/minhook)

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

set(SOURCES
    src/aimbot.cpp
    src/aimbot.h
    src/esp.cpp
    src/esp.h
    src/hooks.cpp
    src/hooks.h
    src/main.cpp
    src/menu.cpp
    src/menu.h
    src/misc.cpp
    src/misc.h
    src/globals.cpp
)
add_library(TrueIssue SHARED ${SOURCES})
target_include_directories(TrueIssue PRIVATE external/imgui external/imgui/backends external/minhook/include)
target_link_libraries(TrueIssue PRIVATE imgui minhook d3d11.lib dxgi.lib)
