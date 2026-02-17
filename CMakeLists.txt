cmake_minimum_required(VERSION 3.16)
project(ImGuiApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_library(imgui STATIC
    OoOVisual/vendor/ImGui/imgui.cpp
    OoOVisual/vendor/ImGui/imgui_draw.cpp
    OoOVisual/vendor/ImGui/imgui_tables.cpp
    OoOVisual/vendor/ImGui/imgui_widgets.cpp
    OoOVisual/vendor/ImGui/imgui_demo.cpp
    OoOVisual/vendor/ImGui/imgui_impl_glfw.cpp
    OoOVisual/vendor/ImGui/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    OoOVisual/vendor/ImGui
    OoOVisual/Core/include/
    OoOVisual/Frontend/include/
    OoOVisual/Visualizer/include/
)

target_link_libraries(imgui PUBLIC
    glfw
    OpenGL::GL
)

add_library(core STATIC
    OoOVisual/Core/src/Commit/ReorderBuffer.cpp
    OoOVisual/Core/src/DCache/DCache.cpp
    OoOVisual/Core/src/Dispatch/Dispatcher.cpp
    OoOVisual/Core/src/Execution/ExecutionStage.cpp
    OoOVisual/Core/src/Execution/ExecutionUnits.cpp
    OoOVisual/Core/src/Fetch/Fetch.cpp
    OoOVisual/Core/src/ReservationStation/ReservationStation.cpp
    OoOVisual/Core/src/ReservationStation/ReservationStationPool.cpp
    OoOVisual/Core/src/RegisterManager/RegisterManager.cpp
    OoOVisual/Core/src/Core.cpp
    OoOVisual/Frontend/src/Parser/Lookup.cpp
    OoOVisual/Frontend/src/Parser/Parser.cpp
    OoOVisual/Visualizer/src/App.cpp
    OoOVisual/Visualizer/src/Camera.cpp
    OoOVisual/Visualizer/src/DrawElementCoreUnit.cpp
    OoOVisual/Visualizer/src/DrawElementDispatcher.cpp
    OoOVisual/Visualizer/src/DrawElementExecutionUnit.cpp
    OoOVisual/Visualizer/src/DrawElementFetchGroup.cpp
    OoOVisual/Visualizer/src/DrawElementFetchUnit.cpp
    OoOVisual/Visualizer/src/DrawElementRegisterManager.cpp
    OoOVisual/Visualizer/src/DrawElementReorderBuffer.cpp
    OoOVisual/Visualizer/src/DrawElementReservationStation.cpp
    OoOVisual/Visualizer/src/DrawElementWire.cpp
    OoOVisual/Visualizer/src/Scene.cpp
    OoOVisual/Visualizer/src/Utils.cpp
)

target_include_directories(core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/OoOVisual/Core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/OoOVisual/Frontend/include
    ${CMAKE_CURRENT_SOURCE_DIR}/OoOVisual/Visualizer/include
)

target_link_libraries(core PUBLIC imgui)

add_executable(cpu_visual
    OoOVisual/main.cpp
)

target_link_libraries(cpu_visual PRIVATE core)