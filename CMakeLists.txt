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
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}
                      ${OPENGL_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      ${GLUT_LIBRARY}
                      glfw
)