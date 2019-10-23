cmake_minimum_required(VERSION 2.8)

project(gltest)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O0 -Wall -Wextra -g")
set(CMAKE_CXX_FLAGS "-O0 -Wall -Wextra -g")
set(CMAKE_CXX_STANDARD 14)
set(ASSIMP_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/outsource/assimp/lib/libassimp.so
					 ${CMAKE_CURRENT_SOURCE_DIR}/outsource/assimp/lib/libIrrXML.a
)

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

set(SRC_FILES src/tests/test_main.cpp
            # src/main.cpp 
              src/framework/misc/debug.cpp 
              src/framework/misc/light.cpp 
              src/framework/misc/camera.cpp 
              src/framework/misc/window.cpp 
			  src/framework/mesh/mesh.cpp
			  src/framework/mesh/model.cpp
			  src/framework/mesh/material.cpp
			  src/framework/base/shader.cpp
              src/framework/base/renderer.cpp 
              src/framework/base/texture.cpp
			  src/framework/base/shadow_map.cpp
              src/framework/base/vertex_array.cpp
              src/framework/base/index_buffer.cpp 
              src/framework/base/vertex_buffer.cpp 
              # tests
              src/tests/test.cpp
              src/tests/texture.cpp
              src/tests/pyramid.cpp
              src/tests/lighting.cpp
              src/tests/clearcolor.cpp
              # libraries
              outsource/imgui/imgui.cpp
              outsource/imgui/imgui_demo.cpp
              outsource/imgui/imgui_draw.cpp
              outsource/imgui/imgui_widgets.cpp
              outsource/imgui/imgui_impl_glfw.cpp
              outsource/imgui/imgui_impl_opengl3.cpp
              outsource/stb_image/stb_image.cpp
)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/hosein/Desktop/bin)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}
					  ${ASSIMP_LIBRARIES}
					  ${OPENGL_LIBRARIES}
                      ${GLEW_LIBRARIES}
					  ${GLUT_LIBRARY}
					  pthread
					  glfw
					  
)