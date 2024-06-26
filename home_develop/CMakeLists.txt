cmake_minimum_required(VERSION 3.0)
project(search_candidates)

# Add all your source files here
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/source.cpp
    ${CMAKE_SOURCE_DIR}/src/callbacks.cpp
    ${CMAKE_SOURCE_DIR}/src/draw.cpp
    ${CMAKE_SOURCE_DIR}/src/branches.cpp
    ${CMAKE_SOURCE_DIR}/src/globals.cpp
    ${CMAKE_SOURCE_DIR}/src/decide_candidates.cpp
)

# Add executable target
add_executable(search_candidates ${SOURCES})

# Link libraries (glfw, OpenGL, GLU, GLEW)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(search_candidates
    glfw
    ${OPENGL_gl_LIBRARY}
    ${GLEW_LIBRARIES}
    GLU
)
