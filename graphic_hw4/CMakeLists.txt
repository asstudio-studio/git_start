cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

message("CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")

# Define project name and detail
project("02 Viewing")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Define the executable name of this project, and create a CMake binary target
add_executable(Light)
add_executable(shade)
add_executable(00933103_HW3)
add_executable(00933103_HW4)
add_executable(00957109_project4)

target_sources(Light PRIVATE "light.c")
target_sources(shade PRIVATE "shade.c")
target_sources(00933103_HW3 PRIVATE "00933103_HW3.c")
target_sources(00933103_HW4 PRIVATE "00933103_HW4.c")
target_sources(00957109_project4 PRIVATE "00957109_project4.c")

# Assign source files to the executable.
target_link_libraries(Light PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)
target_link_libraries(shade PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)
target_link_libraries(00933103_HW3 PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)
target_link_libraries(00933103_HW4 PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)
target_link_libraries(00957109_project4 PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(Light PRIVATE X11)
    target_link_libraries(shade PRIVATE X11)
    target_link_libraries(00933103_HW3 PRIVATE X11)
    target_link_libraries(00933103_HW4 PRIVATE X11)
    target_link_libraries(00957109_project4 PRIVATE X11)
endif()