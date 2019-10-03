cmake_minimum_required(VERSION 2.8)

project(gltest)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O0 -Wall -Wextra -g")
set(CMAKE_CXX_FLAGS "-O0 -Wall -Wextra -g")
set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(include
                    outsource
                    outsource/imgui
                    ${OPENGL_INCLUDE_DIRS}
                    ${GLUT_INCLUDE_DIRS}
                    ${GLFW_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS}
)

set(SRC_FILES src/main.cpp 
              src/gl_utils.cpp 
              src/renderer.cpp 
              src/index_buffer.cpp 
              src/vertex_buffer.cpp 
              src/vertex_array.cpp
              src/shader.cpp
              src/glf_texture.cpp
              outsource/stb_image/stb_image.cpp
              outsource/imgui/imgui.cpp
              outsource/imgui/imgui_demo.cpp
              outsource/imgui/imgui_draw.cpp
              outsource/imgui/imgui_widgets.cpp
              outsource/imgui/imgui_impl_glfw.cpp
              outsource/imgui/imgui_impl_opengl3.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}
                      ${OPENGL_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      ${GLUT_LIBRARY}
                      glfw
)