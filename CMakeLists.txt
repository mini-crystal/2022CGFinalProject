cmake_minimum_required(VERSION 3.10)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(cg_projects VERSION 1.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 14)

file(COPY "media/" DESTINATION "media")

add_subdirectory(./external/glad)

add_subdirectory(./external/glm)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(./external/glfw)

add_subdirectory(./external/tiny_obj_loader)

add_subdirectory(./external/imgui)

add_subdirectory(./external/stb)

add_subdirectory(./projects/2022CGFinalProject)

