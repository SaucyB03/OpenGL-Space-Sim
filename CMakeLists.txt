cmake_minimum_required(VERSION 3.23)
project(OpenGL-Space-Sim)

set(CMAKE_CXX_STANDARD 17)


# ImGui source directory
set(IMGUI_SOURCE_DIR ${CMAKE_SOURCE_DIR}/imgui)

# Add the source files from ImGui
set(IMGUI_SOURCES imgui/imgui_impl_glfw.cpp imgui/imgui_impl_opengl3.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp)

set(PROJECT_SOURCES main.cpp Object.cpp Shader.cpp Camera.cpp stb_image.cpp)

add_executable(OpenGL-Space-Sim ${IMGUI_SOURCES} )

# Include directories
target_include_directories(OpenGL-Space-Sim PRIVATE ${IMGUI_SOURCE_DIR})

target_link_libraries(OpenGL-Space-Sim -lfreeglut -lglew32 -lopengl32 -lglfw3)


