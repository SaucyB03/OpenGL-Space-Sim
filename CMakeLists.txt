cmake_minimum_required(VERSION 3.23)
project(M4OEP-bksoucy)

set(CMAKE_CXX_STANDARD 14)

add_executable(M4OEP-bksoucy main.cpp Object.h Object.cpp Shader.h Shader.cpp Camera.h Camera.cpp stb_image.cpp)
target_link_libraries(M4OEP-bksoucy -lfreeglut -lglew32 -lopengl32 -lglfw3)
