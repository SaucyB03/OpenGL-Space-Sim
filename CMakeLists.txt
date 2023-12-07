cmake_minimum_required(VERSION 3.23)
project(OpenGL-Space-Sim)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCES main.cpp Object.cpp Shader.cpp Camera.cpp stb_image.cpp GUI.cpp GUIElements/guiObject.cpp GUIElements/EnterValue.cpp GUIElements/Button.cpp Text.cpp)

add_executable(OpenGL-Space-Sim ${PROJECT_SOURCES})

find_package(Freetype REQUIRED)

target_link_libraries(OpenGL-Space-Sim -lfreeglut -lglew32 -lopengl32 -lglfw3 Freetype::Freetype)



